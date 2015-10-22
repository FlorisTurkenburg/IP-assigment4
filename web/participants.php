<?php
// Initialize Smarty.
$topdir = realpath(".");
include('smarty/Smarty.class.php');
$smarty = new Smarty;
$smarty->setTemplateDir($topdir . '/smarty/templates');
$smarty->setCompileDir($topdir . '/smarty/templates_c');
$smarty->setCacheDir($topdir . '/smarty/cache');
$smarty->setConfigDir($topdir . '/smarty/configs');

// Load config and assign it to Smarty variables.
include('config.php');
$smarty->assign('phpbase', $WEB_BASEPHP);
$smarty->assign('cgibase', $WEB_BASECGI);
$smarty->assign('hotelgwaddress', $HOTELGW_ADDRESS);
$smarty->assign('hotelgwport', $HOTELGW_PORT);
$smarty->assign('paperaddress', $PAPER_ADDRESS);
$smarty->assign('paperport', $PAPER_PORT);

//for debug purposes
function console_log($data)
{
    echo '<script>';
    echo 'console.log(' . json_encode($data) . ')';
    echo '</script>';
}

//connect to hotel gateway
$fp = fsockopen($HOTELGW_ADDRESS, $HOTELGW_PORT, $errno, $errstr);
if (!$fp) {
    echo "$errstr ($errno)<br />\n";
    console_log("error");
} else {
    fwrite($fp, "g\n");
    $guests = Array();
    $content = "";
    while (!feof($fp)) {
        $char = fgetc($fp);
        $content .= $char;
        if (strlen($content) > 8 && substr($content, -8) == "hotelgw>") {
            break;
        }
    }
    $content = substr($content, 8, -9); //trim "hotelgw>" and '\n' char from the end

    //this check is needed in case there are no participants yet.
    if (strlen($content) > 0) {
        $guests = explode("\n", $content);
    }
    if (count($guests) == 0) {
        $smarty->assign('noguests', true);
    }
    $smarty->assign('guests', $guests);

// Display page.
    $smarty->display('tpl/participants.html');
}
?>
