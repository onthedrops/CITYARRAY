package S2dbobj;

require S2Config;
use DBI;


sub new {

        return $S2dobj::dbobj_instance if($S2dobj::dbobj_instance);
        my $type = shift;
        my $self = {};

        my $returnPointer = bless($self, $type);

        $S2dbobj::dbobj_instance = $returnPointer;

        return $returnPointer;
}


sub getHandle {
	my $self = new S2dbobj;
	my $handleName = shift || 'default';
	my $cacheName = $handleName. $$;
	return $self->{'cache'}->{$cacheName} if($self->{'cache'}->{$cacheName});
	

	$host = S2Config::getConfigValue($handleName . "_host") || 'localhost';
	$db = S2Config::getConfigValue($handleName . '_db');
	$user = S2Config::getConfigValue($handleName . '_user');
	$password = S2Config::getConfigValue($handleName . '_pass');

	print "db connect $handleName -> host [$host] db [$db] user [$user] pass [$password]\n" if($main::debug);

	my $dbh = DBI->connect("dbi:mysql:db=$db;host=$host",$user, $password);
	$self->{'cache'}->{$cacheName} = $dbh;
	return $dbh;
}


1;

