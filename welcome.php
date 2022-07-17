<?php
// print all env variable each in a new line
foreach ($_ENV as $key => $value) {
    echo "$key: $value\n";
}
echo "<pre>";
    echo file_get_contents('php://input');
echo "</pre>\n";
    var_dump( $_POST);
    // sleep(10);