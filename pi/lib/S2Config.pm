package S2Config;

use strict;
use FindBin;

push(@INC,$FindBin::Bin . "/../lib");

#use forks;

require S2Unix;
require S2Log;

use Data::Dumper;
use XML::Simple;
use LWP::UserAgent;

use IO::Socket::SSL;

sub new {

	return $S2Config::SCONFIGinstance if($S2Config::SCONFIGinstance);
	my $type = shift;
	my $self = {};

	my $returnPointer = bless($self, $type);

	$S2Config::SCONFIGinstance = $returnPointer;
	$returnPointer->{'config'} = {};
	$returnPointer->{'_includes'} = 0;

	$returnPointer->loadBaseConfig();

	return $returnPointer;
}

sub getHomedir 
{
	my @path = split(/\//, $FindBin::Bin);
	my $tail;

	while($tail = pop(@path)) {
		if(-d join("/", @path) . "/lib") {
			return join("/", @path);
		}
	}

	return undef;
}



sub readDefaultConfigFile
{
	readConfigFile('/etc/gt/default.xml');
}

sub readConfigIntoPerlHash
{
	my $self = new S2Config;
	my $file = shift;

	my $xs = new XML::Simple;
	my $r = $xs->XMLin($file);

	return $r;
}


sub reloadConfigFiles {
	my $self = new S2Config;

	$self->{'config'} = {};
	my $file;
	my $local;

	foreach $file (@{$self->{'configFiles'}})
	{
		push(@{$local}, $file);
	}

	@{$self->{'configFiles'}} = ();
	foreach $file (@{$local}) {
		S2Config::readConfigFile($file);
	}
}



sub readConfigFile
{

	my $self = new S2Config;
	my $file = shift;
	my $homedir = S2Config::getHomedir();

	push(@{$self->{'configFiles'}}, $file);

	$file =~ s/\{homedir\}/$homedir/g;

	print "S2Config::readConfigFile $file\n" if($main::debug & 65536);

	if($file =~ /^http.*/) {
		return S2Config::readConfigUrl($file);
	}

	if(! -f $file) {
		S2Log::Log(-1,"Warning: can't read $file","S2Config");
		return undef;
	}

	my $xs = new XML::Simple;
	my $r = eval { return $xs->XMLin($file); };

	if(!$r || $@) {
		S2Log::Log(-1,"No return code or error reading $file: $@","S2Config");
	}

	print "Successfully parsed $file\n" if($main::debug & 65536);

	$self->loadKeys($r, $self->{'config'});

	my ($key);
	foreach $key (keys %{$r}) {
		$self->{'config'}->{$key} = $r->{$key};
	}

	$self->{'configFile'} = $r;
}



sub readConfigFileIntoAlternateLocation
{
	my $self = new S2Config;
	my $file = shift;
	my $alternateLocation = shift;

	if($file =~ /^http.*/) {
		return S2Config::readConfigUrlIntoAlternateLocationernateLocation($file, $alternateLocation);
	}

	return 0 if(! -f $file);

	my $xmlParserSimple = new XML::Simple;
	my $parsedXML = $xmlParserSimple->XMLin($file);

# this does not feel even remotely safe to me
#	$self->{$alternateLocation} = {};

	$self->loadKeys($parsedXML, $self->{$alternateLocation});
	$self->{$alternateLocation . '-configFile'} = $parsedXML;

	return 1;

}



sub readConfigUrl
{
	my $self = new S2Config;
	my $url = shift;

 	my $lwp =  new LWP::UserAgent;

        $lwp->timeout(20);

        my $response = $lwp->get($url);

	print "readConfigUrl: Warning unexpected result code: " . $response->{'_rc'} . " for " . $url . "\n" if($response->{'_rc'} ne "200");

        return undef if($response->{'_rc'} ne "200");
        my $xmlParser = new XML::Simple;

        my $parsedXML = $xmlParser->XMLin($response->{'_content'});
	$self->loadKeys($parsedXML, $self->{'config'});
	$self->{'configFile'} = $parsedXML;
}



sub getConfigValueMulti
{

	my $self = new S2Config;
	my $evalString;
	my $argPosition;

	for($argPosition=@_;$argPosition--;$argPosition>0) {
		$evalString .= '->{"' . $_[$argPosition] . '"}';
	}

	$evalString = 'return $self->{"config"}' . $evalString . ' if(defined($self->{"config"}' . $evalString . "));";

	my $returnValue = eval($evalString);

	S2Unix::crash("this case not handled yet") if($returnValue =~ /^HASH.*/);
	return $returnValue;
}



sub setConfigValueMulti
{
	my $self = new S2Config;
	my $value = shift;

	my($argPosition);
	my $evalString = '';

	for($argPosition=@_;$argPosition++;$argPosition<@_) {
		$evalString .= '->{"' . $_[$argPosition] . '"}';
	}

	$evalString = 'return $self->{"config"}' . $evalString . ";";

	my $returnValue = eval($evalString);

	return $returnValue;
}

sub readConfigUrlIntoAlternateLocation
{
	my $self = new S2Config;
	my $url = shift;
	my $alternateLocation = shift;

 	my $lwp =  new LWP::UserAgent;

        $lwp->timeout(20);

        my $response = $lwp->get($url);

	print "readConfigUrl: Warning unexpected resultcode " . $response->{'_rc'} . " for " . $url . "\n" if($response->{'_rc'} ne "200");

        return undef if($response->{'_rc'} ne "200");

        my $xmlParser = new XML::Simple;

        my $parsedXML = $xmlParser->XMLin($response->{'_content'});

	$self->{$alternateLocation} = {};
	$self->loadKeys($parsedXML, $self->{$alternateLocation});
	$self->{$alternateLocation . '-configFile'} = $parsedXML;

	# SAY IT LOUD
	# SAY IT CLEAR
	## YOU CAN LISTEN AS WELL AS YOU HEAR
	# IT'S TOO LATE
	# WHEN WE DIE
	# TO ADMIT WE DON'T SEE EYE TO EYE

	# -- Mike & The Mechanics
	# Dedicated to Nathaniel
	# From SHEER
}

sub readAlt # TODO REFACTOR ME TO BE MORE READABLE
{
	my $self = shift;
	my $spec = shift;
	my ($file, $alt) = split(/,/,$spec);

	readConfigFileIntoAlt($file, $alt);
}



sub loadKeys 
{
	my $self = shift;
	my $r = shift;
	my $t = shift;

	my ($key);

	foreach $key (keys %{$r}) {
		if($key eq "include") {
			if($self->{'_includes'}++ > 100) {
				die "Config error - too many includes";
			}


			if(ref($r->{$key}) eq "ARRAY") {
				my ($file);

				foreach $file (@{$r->{$key}}) {
					readConfigFile($file);
				} 
			} else {
				readConfigFile($r->{$key});
			}
		} elsif($key eq "include-alt") {
			if($self->{'_includes'}++ > 100) {
				die "Config error - too many includes";
			}

			if(ref($r->{$key}) eq "ARRAY") {
				my ($file);


				foreach $file (@{$r->{$key}}) {
					$self->readAlt($file);
				}
			} else {
				$self->readAlt($r->{$key});
			}
		} else {
			$t->{$key} = $r->{$key};
		}
	}
}

# PARENTS - ONE OF THE MOST DESTRUCTIVE THINGS YOU CAN DO TO A CHILD IS FORCE A RELIGION ON THEM
# ANOTHER IS TO TRY TO PREVENT THEM FROM HAVING SEX
# PLEASE STOP BEING SO STUPID
# ALSO BE VERY CAREFUL DURING TOILET TRAINING - YOU DO NOT HAVE A GOOD LINK ESTABLISHED YET - AND IF YOU CARE MORE ABOUT YOUR FURNATURE THAN YOUR CHILDREN YOU ARE NOT READY TO HAVE CHILDREN YET

# SS



sub loadBaseConfig
{
	my $self = shift;

	$self->{'config'}->{'noparent'} = 1;
	$self->{'config'}->{'parents'} = 1;
}



sub configure
{
	my $self = new S2Config;
	my $args = shift;
	$args = $self->{'config'} if(!$args);

	my($arg);

	foreach $arg (keys %{$args}) {

		if($arg eq "debug") {  # debugging modes
			$main::debug = $args->{$arg};
		}

		if($arg eq "xmode") { # running modes
			$main::xmode = $args->{$arg};
		}

		print $arg . " -> " . $self->{'config'}->{$arg} . "\n" if($main::debug & 65536);
	}

	print " xmode: " . $self->interpretXModes() . "\n" if($main::debug);
	print " dmode: " . $self->interpretDModes() . "\n" if($main::debug);
}

sub interpretDModes {
	my $self = shift;
	my $returnMsg = '';
	
	$returnMsg .= 'Generic ' if($main::debug & 1);
	$returnMsg .= 'Kitten ' if($main::debug & 2);
	$returnMsg .= 'Racetrack ' if($main::debug & 4);
	$returnMsg .= 'History ' if($main::debug & 8);
	$returnMsg .= 'Messaging ' if($main::debug & 16);
	$returnMsg .= 'Playhead ' if($main::debug & 32);
	$returnMsg .= 'Genetics ' if($main::debug & 64);
	$returnMsg .= 'Neurophyiscs ' if($main::debug & 128);
	$returnMsg .= 'Paranoid ' if($main::debug & 512); # that would be me..
	$returnMsg .= 'Debugging ' if($main::debug & 32768); # Quis custodiet ipsos custodes?
	$returnMsg .= 'Config ' if($main::debug & 65536);

	return $returnMsg;
}

sub interpretXModes {
	my $self = shift;

	my $returnMsg = '';
	$returnMsg .= 'BuyingEnabled ' if($self::xmode & 1);
	$returnMsg .= 'SellingEnabled ' if($self::xmode & 2);
	$returnMsg .= 'TestModeDisabled ' if($self::xmode & 4);
	$returnMsg .= 'TestFailEnabled ' if($self::xmode & 8);
	$returnMsg .= "Simulation " if(!($self::xmode & 16));
	$returnMsg .= 'ExtraDebugging ' if($self::xmode & 256);

	return $returnMsg;

}

sub configFromArgsEx
{
	my $c;
	@{$c} = @_;
 	configFromArgs(@{$c});
	readConfigFileOrDefault();
	configFromArgs(@{$c});
}

sub configFromEnv # for DOCKER 
{
	S2Config::configure(\%ENV);
}

sub setupDefault # dyslexic, am I?
{
	my $file = shift;
	return S2Config::defaultSetup($file);
}

sub defaultSetup
{
	my $pFile = shift;
	my $appName = S2Unix::getAppName();
	my $appConfigFile = $appName . ".xml";
	my @possibleAppFiles = ( S2Config::getHomedir() . '/conf/' . $appConfigFile, '/etc/gt/' . $appConfigFile );

	my ($file);
	foreach $file (@possibleAppFiles) {
		print "checking $file\n" if($main::debug);

		if(-f $file) {
			S2Config::readConfigFile($file);
		}
	}

	my @possibleDefaults = ( $pFile, S2Config::getHomedir() . '/conf/default.xml', "/etc/gt2/gt.xml", "/etc/gt2.xml" );


	foreach $file (@possibleDefaults) {
		if(-f $file) {
			S2Config::readConfigFile($file);
		}
	}

	my $returnValue = S2Config::setup();

	return $returnValue;
}



sub setup
{
	my $file = shift;

	S2Config::configFromArgs(@main::ARGV);

	my ($configFile);

	if($configFile = S2Config::getConfigValue('configFile')) {
		S2Config::readConfigFile($configFile);
	} else {
		if($file) {
			S2Config::readConfigFile($file);
		}
	}


	S2Config::configFromArgs(@main::ARGV);
	S2Config::configure();
}


sub getCommandLine
{
	return $S2Config::cmdLine;
}


sub configFromArgs
{
	# MUCH MORE BOSS MODE NOW!

	$main::args = {};

	$S2Config::cmdLine = join(' ',@main::ARGV);

	my $argCount = @main::ARGV;
	
	my ($arg);

	foreach $arg(@main::ARGV) { 
		my ($s1, $s2);
		my ($l1, $l2);

		if(($s1,$s2) = $arg =~ /--(.*?)=(.*)/) {
			$s2 = $a if(($a) = $s2 =~ /^\"(.*)\"$/);
			$main::args->{$s1} = $s2;
		} elsif(($s1) = $arg =~ /--(.*)/) {
			$l1 = lc $s1;
			$main::args->{$l1} = 1;
		}
	}

	S2Config::configure($main::args);
}

sub getConfigValue 
{
	my $self = new S2Config;
	my $key = shift;
	my $alt = shift || 'config';
	my $e;	
	my $val = $self->{$alt}->{$key} if(defined $self->{$alt}->{$key});

	if(($e) = $val =~ /!env-(.*)/) {
		$val = $ENV{$e};
	}

# not at all convinced I'd ever want to do this in production

	#if($ENV{'OVERRIDE_' . $key}) {
	#	return $ENV{'OVERRIDE_' . $key};
	#}

	return $val;
}

sub getHostname
{
	return S2Unix::getHostname();
}



sub setConfigValue
{
	my $self = new S2Config;
	my $key = shift;
	my $val = shift;

	$self->{'config'}->{$key} = $val;
}

sub dumpConfiguration
{
	return dumpConfig();
}

sub dumpConfig
{
	my $self = new S2Config;

	return Dumper($self);
}



sub parsePipeColonComma
{
	my $inputString = shift;
	my $r = {};
	my @pipeArray = split(/\|/, $inputString);
	my($pipeSlice);

	foreach $pipeSlice (@pipeArray) {
		my ($key, $value) = split(/\:/, $pipeSlice);
		my @commaArray = split(/,/,$value);

		my ($commaValue);
		foreach $commaValue (@commaArray) {
			$r->{$key}->{$commaValue}++;
		}
	}

	return $r;
}

# I LOVE YOU VICKY

1;

