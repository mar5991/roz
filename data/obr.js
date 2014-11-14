var linie = [];
var canvas = document.getElementById('mycanvas');
var canvas2 = document.getElementById('canvas2');
var ctx = canvas.getContext('2d');
var cty = canvas2.getContext('2d');
var myoffsetX = 0;
var myoffsetY = 0;
var zoom = 1;
function defineLineAsRect(x1, y1, x2, y2, lineWidth) 
{
	var dx = x2 - x1; // deltaX used in length and angle calculations 
	var dy = y2 - y1; // deltaY used in length and angle calculations
	var lineLength = Math.sqrt(dx * dx + dy * dy);
	var lineRadianAngle = Math.atan2(dy, dx);
	console.log('siemanko '+x1+' '+y1);
	return ({
		translateX: x1,
		translateY: y1,
		rotation: lineRadianAngle,
		rectX: 0,
		rectY: -lineWidth / 2,
		rectWidth: lineLength,
		rectHeight: lineWidth
	});
}

function drawLineAsRect(lineAsRect, color) 
{
	var r = lineAsRect;
	ctx.save();
	ctx.beginPath();
	ctx.translate(r.translateX, r.translateY);
	ctx.rotate(r.rotation);
	ctx.rect(r.rectX, r.rectY, r.rectWidth, r.rectHeight);
	ctx.translate(-r.translateX, -r.translateY);
	ctx.rotate(-r.rotation);
	ctx.fillStyle = color;
	ctx.strokeStyle = color;
	ctx.fill();
	ctx.stroke();
	ctx.restore();
}
function whatLineClicked(mouseX, mouseY)
{
	for(i=0; i<linie.length; i++)
	{
		var linet=new defineLineAsRect((linie[i].x1-myoffsetX)*zoom, (linie[i].y1-myoffsetY)*zoom, (linie[i].x2-myoffsetX)*zoom, (linie[i].y2-myoffsetY)*zoom, 4);
		console.log('coona '+linet.rotation);
		drawLineAsRect(linet, 'transparent');
		if (ctx.isPointInPath(mouseX, mouseY)) 
		{
			return i;
		}
	}
	return -1;
}

function handleMouseMove(e) 
{

	console.log(linie.length);
	console.log('lol2 '+e.clientX+' '+e.clientY+' '+canvas.offsetX+' '+canvas.offsetY);
	if(e.offsetX) 
	{
		mouseX = e.offsetX;
		mouseY = e.offsetY;
	}
	else if(e.layerX) 
	{
		mouseX = e.layerX;
		mouseY = e.layerY;
	}
	console.log('lol2 '+mouseX+' '+mouseY);
	console.log('lol23 '+e.clientX+' '+e.clientY+' '+canvas.offsetX+' '+canvas.offsetY);
	var numb=whatLineClicked(mouseX, mouseY);
	canvas2.width = canvas2.width;
	console.log('lol55 '+e.clientX+' '+e.clientY+' '+canvas.offsetX+' '+canvas.offsetY);
	
		console.log(numb);
	if(numb==-1)
	{
		document.body.style.cursor = "";
	}
	else
	{
		console.log('fas '+linie[numb].podobne);
		console.log('fas '+linie[numb].podobne.size);
		document.body.style.cursor = "pointer";
		for(i=0; i<linie[numb].podobne.length; i++)
		{
			cty.beginPath();
			cty.lineWidth = 5;
			cty.moveTo((linie[linie[numb].podobne[i]].x1-myoffsetX)*zoom, (linie[linie[numb].podobne[i]].y1-myoffsetY)*zoom);
			cty.lineTo((linie[linie[numb].podobne[i]].x2-myoffsetX)*zoom, (linie[linie[numb].podobne[i]].y2-myoffsetY)*zoom);
			cty.strokeStyle = '#ff0000';
			cty.stroke();
		}
	}
}
	
function handleMouseClick(e) 
{
	console.log('lol3 '+e.clientX+' '+e.clientY+' '+canvas.offsetX+' '+canvas.offsetY);
	if(e.offsetX)
	{
		mouseX = e.offsetX;
		mouseY = e.offsetY;
	}
	else if(e.layerX)
	{
		mouseX = e.layerX;
		mouseY = e.layerY;
	}
	var numb=whatLineClicked(mouseX, mouseY);
	cty.clearRect(0, 0, canvas2.width, canvas2.height);
	if(numb!=-1)
	{
		window.location=linie[i].link;
	}
}
function repaint()
{
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	cty.clearRect(0, 0, canvas2.width, canvas2.height);
	for (i=0;i<linie.length;i++)
	{
		ctx.beginPath()
		ctx.lineWidth = 1;
		if(linie[i].link==-1)
		{
			ctx.strokeStyle = '#00ff00';
		}
		else
		{
			ctx.strokeStyle = '#000000';
		}
		ctx.moveTo((linie[i].x1-myoffsetX)*zoom, (linie[i].y1-myoffsetY)*zoom);
		ctx.lineTo((linie[i].x2-myoffsetX)*zoom, (linie[i].y2-myoffsetY)*zoom);
		ctx.stroke();
	}

}
function updateinfo()
{
	myoffsetX = parseInt(document.getElementById("xwal").value, 10);
	myoffsetY = parseInt(document.getElementById("ywal").value, 10);
	zoom = parseFloat(document.getElementById("zwal").value, 10);
	repaint();
}
function init()
{
	var str1=document.getElementById("lines").innerText;
	console.log(str1);
	linie=JSON.parse(str1);
	/*for(i=0; i<30; i++)
	{
		linie[i]={link: 'http://facebook.com', x1: Math.floor(Math.random()*300), y1:Math.floor(Math.random()*300), x2:Math.floor(Math.random()*300), y2:Math.floor(Math.random()*300)};
	}*/
	canvas.addEventListener('mousemove', handleMouseMove, false);
	canvas.addEventListener('click', handleMouseClick, false);
	canvas2.addEventListener('mousemove', handleMouseMove, false);
	canvas2.addEventListener('click', handleMouseClick, false);
}
init();
repaint();
