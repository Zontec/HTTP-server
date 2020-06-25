function showContent(id) {

	var temp = document.getElementById(id);
  	var clon = temp.content.cloneNode(true);
  	document.getElementById("main").innerHTML = "";
  	document.getElementById("main").appendChild(clon)
}

showContent('news');