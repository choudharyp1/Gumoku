/**
 * Created by Pratik on 9/20/2016.
 */

$(document).ready(function () {
    $('#gomoku td').click(function () {
        $(this).addClass('addimage');
    })
})


function countDown(secs,elem) {
	var element = document.getElementById(elem);
	element.innerHTML = '<h4 style="font-family: serif; font-size: 20px;">You have <b>'+secs+'</b> seconds to play your move</h4>';


    if (secs <= 0){
        clearTimeout(timer);
        alert("User has timed out. Start a New Game.");
        return;
    }
    else {
        secs--;
    }
	var timer = setTimeout('countDown('+secs+',"'+elem+'")',1000);
}