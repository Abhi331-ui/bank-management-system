// NexBank Theme & Notifications Controller
// Persists Dark Mode preferences and handles interactive notification dropdowns across all pages.

(function() {
    // Inject dark mode stylesheet dynamically
    const link = document.createElement("link");
    link.rel = "stylesheet";
    link.href = "dark_mode.css";
    document.head.appendChild(link);

    // 1. Theme persistence and dark mode setup (Immediate Execution to prevent FOUC)
    const storedTheme = localStorage.getItem("theme");
    const systemPrefersDark = window.matchMedia("(prefers-color-scheme: dark)").matches;
    
    if (storedTheme === "dark" || (!storedTheme && systemPrefersDark)) {
        document.documentElement.classList.add("dark");
    } else {
        document.documentElement.classList.remove("dark");
    }

    // Expose utility functions globally
    window.addNotification = function(title, message) {
        const session = localStorage.getItem("nexbank_current_user");
        if (!session) return;
        const currentUser = JSON.parse(session);
        const storageKey = `nexbank_notifications_${currentUser.account_number}`;
        
        const list = JSON.parse(localStorage.getItem(storageKey) || "[]");
        list.unshift({
            id: Date.now(),
            title: title,
            message: message,
            time: new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' }),
            read: false
        });
        localStorage.setItem(storageKey, JSON.stringify(list));
        
        // Sync active views
        renderNotificationsUI();
    };

    window.clearAllNotifications = function() {
        const session = localStorage.getItem("nexbank_current_user");
        if (!session) return;
        const currentUser = JSON.parse(session);
        const storageKey = `nexbank_notifications_${currentUser.account_number}`;
        localStorage.setItem(storageKey, "[]");
        renderNotificationsUI();
    };

    // Initialize DOM handlers
    document.addEventListener("DOMContentLoaded", () => {
        setupThemeToggle();
        setupNotificationsUI();
    });

    // Theme Toggle setup
    function setupThemeToggle() {
        const toggleBtn = document.getElementById("theme-toggle");
        if (!toggleBtn) return;

        // Sync button content initially
        updateToggleIcon(toggleBtn);

        toggleBtn.addEventListener("click", () => {
            const isDark = document.documentElement.classList.toggle("dark");
            localStorage.setItem("theme", isDark ? "dark" : "light");
            updateToggleIcon(toggleBtn);
        });
    }

    function updateToggleIcon(btn) {
        const iconSpan = btn.querySelector("span") || btn;
        const isDark = document.documentElement.classList.contains("dark");
        if (iconSpan && iconSpan.classList.contains("material-symbols-outlined")) {
            // Check if page expects 'contrast' or 'dark_mode'
            if (iconSpan.innerText === "contrast") {
                iconSpan.innerText = "contrast";
            } else {
                iconSpan.innerText = isDark ? "light_mode" : "dark_mode";
            }
        }
    }

    // Notifications Dropdown and List Management
    function setupNotificationsUI() {
        const btn = document.getElementById("notification-btn");
        const dropdown = document.getElementById("notification-dropdown");
        const markReadBtn = document.getElementById("mark-all-read-btn");

        if (!btn || !dropdown) return;

        // Close dropdown when clicking outside
        document.addEventListener("click", (e) => {
            if (!btn.contains(e.target) && !dropdown.contains(e.target)) {
                dropdown.classList.add("hidden");
            }
        });

        btn.addEventListener("click", () => {
            dropdown.classList.toggle("hidden");
            if (!dropdown.classList.contains("hidden")) {
                markAllAsReadSilently();
            }
        });

        if (markReadBtn) {
            markReadBtn.addEventListener("click", () => {
                markAllAsReadSilently();
            });
        }

        renderNotificationsUI();
    }

    function renderNotificationsUI() {
        const session = localStorage.getItem("nexbank_current_user");
        if (!session) return;
        const currentUser = JSON.parse(session);
        const storageKey = `nexbank_notifications_${currentUser.account_number}`;
        const list = JSON.parse(localStorage.getItem(storageKey) || "[]");

        const badge = document.getElementById("notification-badge");
        const listContainer = document.getElementById("notification-list");

        // Prepopulate welcome message if empty list
        if (list.length === 0 && !localStorage.getItem(`nexbank_notif_welcome_${currentUser.account_number}`)) {
            list.push({
                id: 1,
                title: "Welcome to NexBank",
                message: "Your institutional capital account is active and verified under SOC2 security standards.",
                time: "12:00 PM",
                read: false
            });
            localStorage.setItem(storageKey, JSON.stringify(list));
            localStorage.setItem(`nexbank_notif_welcome_${currentUser.account_number}`, "true");
        }

        // Render badge
        const unreadCount = list.filter(x => !x.read).length;
        if (badge) {
            if (unreadCount > 0) {
                badge.classList.remove("hidden");
                badge.textContent = unreadCount > 9 ? "9+" : unreadCount;
                // Add badge text styles if not already present
                badge.className = "absolute top-1 right-1 w-4 h-4 bg-error text-white text-[9px] font-bold rounded-full flex items-center justify-center border border-white";
            } else {
                badge.classList.add("hidden");
            }
        }

        // Render list content
        if (listContainer) {
            listContainer.innerHTML = "";
            if (list.length === 0) {
                listContainer.innerHTML = `
                    <div class="p-lg text-center text-on-surface-variant text-body-sm opacity-60">
                        No new notifications.
                    </div>
                `;
            } else {
                list.forEach(item => {
                    const row = document.createElement("div");
                    row.className = `p-md transition-colors duration-150 ${item.read ? 'bg-transparent' : 'bg-primary/5 dark:bg-primary/10'}`;
                    row.innerHTML = `
                        <div class="flex flex-col gap-xs">
                            <div class="flex justify-between items-start">
                                <span class="font-label-md text-label-md font-bold text-on-surface">${item.title}</span>
                                <span class="text-[10px] text-outline">${item.time}</span>
                            </div>
                            <p class="text-body-sm text-body-sm text-on-surface-variant leading-tight">${item.message}</p>
                        </div>
                    `;
                    listContainer.appendChild(row);
                });
            }
        }
    }

    function markAllAsReadSilently() {
        const session = localStorage.getItem("nexbank_current_user");
        if (!session) return;
        const currentUser = JSON.parse(session);
        const storageKey = `nexbank_notifications_${currentUser.account_number}`;
        const list = JSON.parse(localStorage.getItem(storageKey) || "[]");

        list.forEach(x => x.read = true);
        localStorage.setItem(storageKey, JSON.stringify(list));
        
        // Hide badge only, leave background highlight until clear/refresh
        const badge = document.getElementById("notification-badge");
        if (badge) badge.classList.add("hidden");
    }
})();
