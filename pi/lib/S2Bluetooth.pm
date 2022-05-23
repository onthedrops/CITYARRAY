package S2Bluetooth;

use Net::Bluetooth;
use IO::Handle;
use Fcntl;


sub new {
	my $self = {};
	my $type = shift;
	bless($self, $type);
}

sub getSigns {
	my $self = shift;
	my $devices = get_remote_devices();
	
	my $return_devices = {};
	foreach $addr (keys %{$devices}) {
		if($devices->{$addr} =~ /SIGN.*/) {
			$return_devices->{$addr} = $devices->{$addr};
		}
	}
	
	return $return_devices;
}

sub Disconnect {
	my $self = shift;
	$self->{'btport'}->close();
	$self->{'btfh'} = undef;
}

sub Connect {
	my $self = shift;
	my $addr = shift;

	$self->{'btport'} = Net::Bluetooth->newsocket("RFCOMM");

	if($self->{'btport'}->connect($addr, 1) != 0) {
		return 0;
	}

	$self->{'btfh'} = $self->{'btport'}->perlfh();
	$self->{'btfh'}->autoflush(1);
	my $flags = fcntl($self->{'btfh'}, F_GETFL, 0);
	fcntl($self->{'btfh'}, F_SETFL, $flags | O_NONBLOCK);
	sleep(1);

}

sub putLine {
	my $self = shift;
	my $cmd = shift;
	my $fh = $self->{'btfh'};


	print $fh "$cmd\n";
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

	return $self->getValue();
}


1;

