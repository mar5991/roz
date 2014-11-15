var linie = [];
var ytics = [];
var canvas = document.getElementById('mycanvas');
var canvas2 = document.getElementById('canvas2');
var canvas3 = document.getElementById('canvas3');
var ctx = canvas.getContext('2d');
var cty = canvas2.getContext('2d');
var ctz = canvas3.getContext('2d');
var minX = 0;
var minY = 0;
var maxX = 1000;
var maxY = 0;
var myoffsetX = 0;
var myoffsetY = 0;
var zoomX = 1;
var zoomY = 1;
var clickCzahaX = 0;
var clickCzahaY = 0;
var clickTryb = 0;
function toMatrixX(wart)
{
	return ((wart-myoffsetX)*zoomX);
}
function toMatrixY(wart)
{
	return ((wart-myoffsetY)*zoomY);
}
function fromMatrixX(wart)
{
	return wart/zoomX+myoffsetX;
}
function fromMatrixY(wart)
{
	return wart/zoomY+myoffsetY;
}
function setMinMax()
{
	for(i=0; i<linie.length; i++)
	{
		maxY=Math.max(maxY, linie[i].y1);
		maxY=Math.max(maxY, linie[i].y2);
	}
}
function setRegion(x1, x2, y1, y2)
{
	//canvas.width canvas.height
}
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
		var linet=new defineLineAsRect(toMatrixX(linie[i].x1), toMatrixY(linie[i].y1), toMatrixX(linie[i].x2), toMatrixY(linie[i].y2), 4);
		console.log('coona '+linet.rotation);
		drawLineAsRect(linet, 'transparent');
		if (ctx.isPointInPath(mouseX, mouseY)) 
		{
			return i;
		}
	}
	return -1;
}

function totimemin(wart)
{
	var wynik='';
	wynik+=parseInt(wart/3600, 10);
	var wart2=wart%3600;
	wart2/=60;
	wynik+=':';
	if(wart2<10)
	{
		wynik+='0';
	}
	wynik+=parseInt(wart2, 10);
	return wynik;
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
			cty.moveTo(toMatrixX(linie[linie[numb].podobne[i]].x1), toMatrixY(linie[linie[numb].podobne[i]].y1));
			cty.lineTo(toMatrixX(linie[linie[numb].podobne[i]].x2), toMatrixY(linie[linie[numb].podobne[i]].y2));
			cty.strokeStyle = '#ff0000';
			cty.stroke();
		}
		cty.fillStyle = '#ff0000';
		cty.font = '16px sans-serif';
		cty.textBaseline = 'bottom';
		console.log(linie[numb].fromtime+' xxx '+linie[numb].totime);
		cty.fillText(linie[numb].from+' '+totimemin(linie[numb].fromtime)+' - '+linie[numb].to+' '+totimemin(linie[numb].totime), mouseX+10, mouseY+10);
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
		clickTryb=0;
	}
	else
	{
		console.log("SIEJBIK TO DEBIL");
		if(clickTryb==0)
		{
			clickCzahaX=mouseX;
			clickCzahaY=mouseY;
			clickTryb=1;
		}
		else if(clickTryb==1)
		{
			var x1 = fromMatrixX(clickCzahaX);
			var y1 = fromMatrixY(clickCzahaY);
			var x2 = fromMatrixX(mouseX);
			var y2 = fromMatrixY(mouseY);
			console.log('SIEJBIK TORONTO '+x1+' '+y1+' '+x2+' '+y2);
			var xLeft = Math.min(x1,x2);
			var xRight = Math.max(x1,x2);
			var yTop = Math.min(y1,y2);
			var yBottom = Math.max(y1,y2);
			myoffsetX=xLeft;
			myoffsetY=yTop;
			zoomX=1/(xRight-xLeft)*canvas.width;
			zoomY=1/(yBottom-yTop)*canvas.height;
			clickTryb=0;
			repaint();
		}
	}
}
function yTicsRepaint()
{
	var czest=60*60;
	var start=parseInt(fromMatrixX(0)/czest, 10);
	var stop=parseInt(fromMatrixX(canvas3.width)/czest, 10);
	console.log(start+' st '+stop);
	for(i=start; i<=stop; i++)
	{
		ctx.beginPath()
		ctx.strokeStyle = '#006600';
		ctx.lineWidth = 1;
		ctx.moveTo(toMatrixX(i*czest), 0);
		ctx.lineTo(toMatrixX(i*czest), canvas3.height);
		ctx.stroke();
		ctz.fillStyle = '#006600';
		ctz.font = '10px sans-serif';
		ctz.textBaseline = 'top';
		ctz.fillText(i, toMatrixX(i*czest), 5);
	}
}
function repaint()
{
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	cty.clearRect(0, 0, canvas2.width, canvas2.height);
	ctz.clearRect(0, 0, canvas3.width, canvas3.height);
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
		ctx.moveTo(toMatrixX(linie[i].x1), toMatrixY(linie[i].y1));
		ctx.lineTo(toMatrixX(linie[i].x2), toMatrixY(linie[i].y2));
		ctx.stroke();
	}
	for(i=0; i<ytics.length; i++)
	{
		ctz.fillStyle = '#006600';
		ctz.font = '10px sans-serif';
		ctz.textBaseline = 'bottom';
		if(i==0)
			ctz.textBaseline = 'top';
		ctz.fillText(ytics[i].name, 5, toMatrixY(ytics[i].poz));
		ctx.beginPath()
		ctx.strokeStyle = '#006600';
		ctx.lineWidth = 1;
		ctx.moveTo(0, toMatrixY(ytics[i].poz));
		ctx.lineTo(canvas3.width, toMatrixY(ytics[i].poz));
		ctx.stroke();
	}
	yTicsRepaint();
}
function clearzoom()
{
	zoomX=1/(1440*60)*canvas.width;
	zoomY=1/maxY*canvas.height;
	myoffsetX=0;
	myoffsetY=0;
	repaint();
}
 window.addEventListener('resize', resizeCanvas, false);

function resizeCanvas()
{
	canvas.width = window.innerWidth;
	canvas2.width = window.innerWidth;
	canvas3.width = window.innerWidth;
	canvas.height = Math.min(600, window.innerWidth);
	canvas2.height = Math.min(600, window.innerWidth);
	canvas3.height = Math.min(600, window.innerWidth);
	repaint();
}
function init()
{
	canvas.width = window.innerWidth;
	canvas2.width = window.innerWidth;
	canvas3.width = window.innerWidth;
	canvas.height = Math.min(600, window.innerWidth);
	canvas2.height = Math.min(600, window.innerWidth);
	canvas3.height = Math.min(600, window.innerWidth);
	
	var str1=document.getElementById("lines").innerText;
	linie=JSON.parse(str1);
	var str2=document.getElementById("ytics").innerText;
	ytics=JSON.parse(str2);
	setMinMax();
	zoomX=1/(1440*60)*canvas.width;
	zoomY=1/maxY*canvas.height;
	console.log("ZOOMX"+zoomX);
	/*for(i=0; i<30; i++)
	{
		linie[i]={link: 'http://facebook.com', x1: Math.floor(Math.random()*300), y1:Math.floor(Math.random()*300), x2:Math.floor(Math.random()*300), y2:Math.floor(Math.random()*300)};
	}*/
	canvas.addEventListener('mousemove', handleMouseMove, false);
	canvas.addEventListener('click', handleMouseClick, false);
	canvas2.addEventListener('mousemove', handleMouseMove, false);
	canvas2.addEventListener('click', handleMouseClick, false);
	canvas3.addEventListener('mousemove', handleMouseMove, false);
	canvas3.addEventListener('click', handleMouseClick, false);
}
init();
repaint();
