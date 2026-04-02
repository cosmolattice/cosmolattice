// Replace emgithub's "view raw" links (jsDelivr) with GitHub blob links.
// Runs after emgithub embeds have loaded.
new MutationObserver(function (mutations) {
  document.querySelectorAll('.emgithub-file .file-meta a[href*="cdn.jsdelivr.net"]').forEach(function (a) {
    // jsDelivr URL: https://cdn.jsdelivr.net/gh/{user}/{repo}@{branch}/{path}
    var m = a.href.match(/cdn\.jsdelivr\.net\/gh\/([^/]+)\/([^@]+)@([^/]+)\/(.+)/);
    if (m) {
      a.href = 'https://github.com/' + m[1] + '/' + m[2] + '/blob/' + m[3] + '/' + m[4];
      a.style.display = '';
    }
  });
}).observe(document.body, { childList: true, subtree: true });
