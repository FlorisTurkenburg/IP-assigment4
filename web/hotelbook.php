<?php
// Initialize Smarty.
$topdir=realpath(".");
include('smarty/Smarty.class.php');
$smarty = new Smarty;
$smarty->setTemplateDir($topdir.'/smarty/templates');
$smarty->setCompileDir($topdir.'/smarty/templates_c');
$smarty->setCacheDir($topdir.'/smarty/cache');
$smarty->setConfigDir($topdir.'/smarty/configs');

// Load config and assign it to Smarty variables.
include('config.php');
$smarty->assign('phpbase', $WEB_BASEPHP);
$smarty->assign('cgibase', $WEB_BASECGI);
$smarty->assign('hotelgwaddress', $HOTELGW_ADDRESS);
$smarty->assign('hotelgwport', $HOTELGW_PORT);
$smarty->assign('paperaddress', $PAPER_ADDRESS);
$smarty->assign('paperport', $PAPER_PORT);

//for debug purposes
function console_log( $data ){
    echo '<script>';
    echo 'console.log('. json_encode( $data ) .')';
    echo '</script>';
}

//if there are any console_log calls before this, it won't work
function redirect($url, $statusCode = 303)
{
    header('Location: ' . $url, true, $statusCode);
    exit();
}

function pop_flash_message() {
    $msg = "";
    if (isset($_SESSION['flash_msg'])) {
        $msg = $_SESSION['flash_msg'];
        $_SESSION['flash_msg'] = "";
    }
    return $msg;
}
ini_set('display_errors', 'On'); //debugging

if($_SERVER['REQUEST_METHOD'] == 'GET') {
    //Add error/success messages if there is any
    session_start();
    $flash_msg = pop_flash_message();
    $success = isset($_SESSION['success']) && $_SESSION['success'];
    $smarty->assign('flash_msg', $flash_msg);
    $smarty->assign('success', $success);
    // Display page.
    $smarty->display('tpl/hotelbook.html');
}

if($_SERVER['REQUEST_METHOD'] == 'POST' &&
    isset($_POST['type']) &&
    isset($_POST['name'])){
    session_start();
    $_SESSION['success'] = null;
    //form validation
    $type = $_POST['type'];
    if($type != 1 && $type != 2 && $type != 3 ){
        $_SESSION['flash_msg'] = "Wrong room type";
        $_SESSION['success'] = false;
        redirect($WEB_BASEPHP."/hotelbook.php");
    }

    $name = $_POST['name'];

    //connect to gateway
    $fp = fsockopen($HOTELGW_ADDRESS, $HOTELGW_PORT, $errno, $errstr);
    if (!$fp) {
        console_log("$errstr ($errno)<br />\n");
    } else {
        $hotelgw = fgets($fp,9); //capturing the first prompt
        $gw_request = "b ".$type." ".$name."\n";
        fwrite($fp, $gw_request);
        $response = fgets($fp);
        $secondgw = fgets($fp,9); //capturing the second prompt
        fclose($fp);
    }

    $response = substr($response, 0, -1); //trim the new line char at the end
    $_SESSION['flash_msg'] = "Successfully booked room type:".$type." for: ".$name;
    $_SESSION['success'] = true;

    //redirect to GET hotelbook.php
    redirect($WEB_BASEPHP."/hotelbook.php");
}

?>