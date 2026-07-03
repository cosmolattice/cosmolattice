document.addEventListener("DOMContentLoaded", function () {
  const title = document.querySelector(".md-header__title");

  if (!title) return;

  const newsBar = document.createElement("div");
  newsBar.className = "cl-news-inline";

  newsBar.innerHTML = `
    <span class="cl-news-label">News</span>

    <span class="cl-news-item active">
      🚀 v2.0 released — <a href="/code/">read more</a>
    </span>

    <span class="cl-news-item">
      🎓 CL School announced — <a href="/cl-schools/">details</a>
    </span>

    <span class="cl-news-item">
      📝 New publication — <a href="/publications/">see more</a>
    </span>
  `;

  title.insertAdjacentElement("afterend", newsBar);

  const items = newsBar.querySelectorAll(".cl-news-item");
  let index = 0;

  setInterval(() => {
    items[index].classList.remove("active");
    index = (index + 1) % items.length;
    items[index].classList.add("active");
  }, 4500);
});