document.addEventListener("DOMContentLoaded", function () {
  const title = document.querySelector(".md-header__title");

  if (!title) return;

  // Resolve links from the documentation root, independently of the page
  // currently being viewed and of any deployment subdirectory.
  const script = Array.from(document.scripts).find((element) =>
    element.src.includes("/javascripts/news-bar.js")
  );
  const docsRoot = new URL("../", script ? script.src : document.baseURI);
  const docsUrl = (path) => new URL(path, docsRoot).href;

  const newsBar = document.createElement("div");
  newsBar.className = "cl-news-inline";

  newsBar.innerHTML = `
    <span class="cl-news-label">News</span>

	<span class="cl-news-item active">
		🚀 CL v2.0 released — <a href="${docsUrl("CL_News/CLnews.html#cl-v2-release")}">read more</a>
	</span>

	<span class="cl-news-item">
		🎓 CL Workshop 2026 — <a href="${docsUrl("CL_News/CLnews.html#cl-workshop-2026")}">details</a>
	</span>

	<span class="cl-news-item">
		📝 New publications — <a href="${docsUrl("publications/Publications.html")}">see more</a>
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
