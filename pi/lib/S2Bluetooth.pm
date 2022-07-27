package S2Bluetooth;

use Net::Bluetooth;
use IO::Handle;
use Fcntl;


sub new {
	my $self = {};
	my $type = shift;
	$self->{'password'} = 'xyzzy';
	bless($self, $type);
}

sub getSigns {
	my $self = shift;
	my $devices = get_remote_devices();
	
	my $return_devices = {};
	foreach $addr (keys %{$devices}) {
		if($devices->{$addr} =~ /SIGN.*/) {
			$return_devices->{$addr} = $devices->{$addr};
		} elsif($addr =~ /C4:4F.*/) {
			$return_devices->{$addr} = $devices->{$addr};
		} elsif($addr =~ /4C:75.*/) {
			$return_devices->{$addr} = $devices->{$addr};
		} elsif($addr =~ /24:AB.*/) {
			$return_devices->{$addr} = $devices->{$addr};
		}
	}
	
	return $return_devices;
}

sub Disconnect {
	my $self = shift;
	close($self->{'btfh'});
	$self->{'btport'}->close();
	$self->{'btfh'} = undef;
	sleep(1);
}

sub Connect {
	my $self = shift;
	my $addr = shift;


	my $ok = 0;
	$self->{'errs'} = ();

	for(my $retries=0;$retries<3;$retries++) {
		$self->{'btport'} = Net::Bluetooth->newsocket("RFCOMM");
		if($self->{'btport'}->connect($addr, 1) != 0) {
			push(@{$self->{'errs'}}, $!);
			$self->{'err'} = join(",", @{$self->{'errs'}});

			if($! =~ /Operation already in progress/) {
				last;
				$ok = 1;
			} else {
				$self->{'btport'}->close();
			} 
		} else {
			$ok = 1;
			last;
		}
	}

	return 0 if(!$ok);

	$self->{'btfh'} = $self->{'btport'}->perlfh();
	$self->{'btfh'}->autoflush(1);
	my $flags = fcntl($self->{'btfh'}, F_GETFL, 0);
	fcntl($self->{'btfh'}, F_SETFL, $flags | O_NONBLOCK);
	sleep(2);
#	select(undef,undef,undef,0.5);
	return 1;
}

sub reboot {
	my $self = shift;

	my $fh = $self->{'btfh'};

	print $fh "R " . $self->{'password'} . "\n";
}

sub message {
	my $self = shift;
	my $msg = shift;
	my $fh = $self->{'btfh'};

	print $fh "M " . $msg . "\n";
}

	

sub programmingMode {
	my $self = shift;
	
	my $fh = $self->{'btfh'};
	print $fh "p " . $self->{'password'} . "\n";
	sleep(1);
}

sub putLine {
	my $self = shift;
	my $cmd = shift;
	my $fh = $self->{'btfh'};

	if(!$fh) {
		return 0;
	}

	print "Sending $cmd\n" if($main::debug);

	print $fh "$cmd\n";
}

sub getInfo {
	my $self = shift;
	my $buf = "";

	$self->putLine("?");

	for(my $i=0;$i<100;$i++) {
		my $data = $self->getLine();
		#print "$i [$data]\n";
		$data =~ s/\r//g;
		$data =~ s/\n//g;
		$buf .= $data . "\n";
			
		return($buf) if($data eq "EOF");
	}

	return "$buf<<\n";
}


sub getLine {
	my $self = shift;
	$self->{'buffer'} = undef;

	my $done = 0;
	my $error = 0;

	my $startTime = time();


	while(!$done) {
		# todo: timeout handling
		$len = sysread($self->{'btfh'}, $char, 1);
		if($startTime - time() > 10) {
			$done = 1;
			$error = 1;
		}

		next if(!$len);
		if($char eq "\n") {
			$done = 1;
		} else {
			$self->{'buffer'} .= $char;
		}
	}

	$self->{'error'} = $error;

	return $self->{'buffer'};
}

sub getValue {
	my $self = shift;

	return undef if($self->{'error'});

	my ($v) = $self->{'buffer'} =~ /\[(.*)\]/;
	return $v;
}

sub getVersion {
	my $self = shift;
	$self->putLine("V");
	my $v = $self->getLine();
	return $self->getValue();
}

sub getConfigValue {
	my $self = shift;
	my $key = shift;

	$self->putLine("G $key");
	my $v = $self->getLine();
	return undef if($v =~ /-ERR.*/);

	return $self->getValue();
}

sub putConfigValue {
	my $self = shift;
	my $key = shift;
	my $value = shift;

	$self->putLine("S $key,$value");
	my $v = $self->getLine();
	print $v . "\n" if($main::debug);

	return $self->getValue();
}


1;

