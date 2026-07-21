/* People page: click a module chip to filter the roster.
   Progressive enhancement — the HTML already contains every chip and card, so
   with JS disabled the full roster and all badges still render (just without
   filtering). This script only wires the interaction. Filtering is union / OR:
   a person shows if they carry ANY of the active modules.

   No navigation.instant in this theme, so a normal DOMContentLoaded is enough;
   the readyState guard also covers the script loading after the DOM is parsed. */
(function () {
  "use strict";

  function initPeopleFilter() {
    var root = document.querySelector(".cl-people");
    if (!root || root.dataset.filterReady) return;
    root.dataset.filterReady = "1";

    var active = new Set();
    var chips = Array.prototype.slice.call(root.querySelectorAll(".cl-ppl-chip"));
    var cards = Array.prototype.slice.call(root.querySelectorAll(".cl-ppl-card"));
    var sections = Array.prototype.slice.call(root.querySelectorAll(".cl-ppl-section"));
    var shownEl = root.querySelector("[data-count-shown]");
    var totalEl = root.querySelector("[data-count-total]");
    var clearBtn = root.querySelector(".cl-ppl-clear");
    var prefersReduce = window.matchMedia("(prefers-reduced-motion: reduce)").matches;

    if (totalEl) totalEl.textContent = String(cards.length);

    function cardMods(card) {
      return (card.getAttribute("data-mods") || "").trim().split(/\s+/);
    }

    function matches(mods) {
      if (active.size === 0) return true;
      for (var i = 0; i < mods.length; i++) {
        if (active.has(mods[i])) return true;
      }
      return false;
    }

    function render() {
      chips.forEach(function (c) {
        var on = active.has(c.getAttribute("data-mod"));
        c.classList.toggle("is-active", on);
        c.setAttribute("aria-pressed", on ? "true" : "false");
      });

      var shown = 0;
      cards.forEach(function (card) {
        var mods = cardMods(card);
        var show = matches(mods);
        if (show) shown++;

        Array.prototype.forEach.call(card.querySelectorAll(".cl-ppl-chip"), function (c) {
          c.classList.toggle("is-match", active.size > 0 && active.has(c.getAttribute("data-mod")));
        });

        if (show) {
          card.hidden = false;
          if (prefersReduce) card.classList.remove("is-leaving");
          else requestAnimationFrame(function () { card.classList.remove("is-leaving"); });
        } else if (prefersReduce) {
          card.hidden = true;
        } else {
          card.classList.add("is-leaving");
          setTimeout(function () {
            if (!matches(cardMods(card))) card.hidden = true;
          }, 200);
        }
      });

      sections.forEach(function (s) {
        var any = Array.prototype.some.call(s.querySelectorAll(".cl-ppl-card"), function (c) {
          return matches(cardMods(c));
        });
        s.hidden = !any;
      });

      if (shownEl) shownEl.textContent = String(shown);
      if (clearBtn) clearBtn.disabled = active.size === 0;
    }

    function toggle(key) {
      if (!key) return;
      if (active.has(key)) active.delete(key);
      else active.add(key);
      render();
    }

    chips.forEach(function (c) {
      c.addEventListener("click", function () { toggle(c.getAttribute("data-mod")); });
    });
    if (clearBtn) {
      clearBtn.addEventListener("click", function () { active.clear(); render(); });
    }

    render();
  }

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", initPeopleFilter);
  } else {
    initPeopleFilter();
  }
})();
