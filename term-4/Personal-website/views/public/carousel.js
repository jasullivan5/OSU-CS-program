document.addEventListener('DOMContentLoaded', wrapper);

function wrapper(){
  var index = 0;
  function carousel(){
    var slides = document.getElementsByClassName("slides");
    for (var i = 0; i < slides.length; ++i){
      slides[i].style.display = "none";
    }
    index++;
    if (index >= slides.length) {
      index = 0;
    }
    slides[index].style.display = "block";
  }
  carousel();
  setInterval(carousel, 3000);
}
