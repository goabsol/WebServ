<?php

    session_start();

    $_SESSION["name"] = "111";
    $cookie_name = "name";
    $cookie_value = "Anas";
    setcookie("name", "ANAS", time() + (86400 * 30), "/"); // 86400 = 1 day
    setcookie("role", "Socketa", time() + (86400 * 30), "/"); // 86400 = 1 day
    echo "Session and cookies has been set :D";
?>