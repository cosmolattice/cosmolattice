document.addEventListener("DOMContentLoaded", function () {
  if (window.innerWidth < 1220) return;

  var sidebar = document.querySelector(".md-sidebar--primary");
  if (!sidebar) return;

  var btn = document.createElement("button");
  btn.className = "sidebar-toggle-btn";
  btn.title = "Toggle navigation panel";
  document.body.appendChild(btn);

  function applyState(collapsed) {
    document.body.classList.toggle("nav-collapsed", collapsed);
    btn.textContent = collapsed ? "›" : "‹";
  }

  applyState(false);

  btn.addEventListener("click", function () {
    applyState(!document.body.classList.contains("nav-collapsed"));
  });
});
