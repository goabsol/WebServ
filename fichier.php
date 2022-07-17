<?php
$stuff = <<<END_OF_STUFF
\n\n\n\n\n\n
helloGOOD
\n\n\n\n\n\n
END_OF_STUFF;

$fh = fopen('php://stderr','a'); //both (a)ppending, and (w)riting will work

// $name = $_POST['name'];

// echo $name;
// echo "mook";
phpinfo();
// fwrite(STDERR, "DONE\n");
fwrite($fh,$stuff);
fclose($fh);
?>