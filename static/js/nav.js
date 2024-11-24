function toggleProfileMenu() {
    const menu = document.getElementById('profileMenu');
    menu.classList.toggle('hidden');

    // Close menu when clicking outside
    document.addEventListener('click', function closeMenu(e) {
        const menu = document.getElementById('profileMenu');
        const button = e.target.closest('button');
        
        if (!button && !menu.classList.contains('hidden')) {
            menu.classList.add('hidden');
            document.removeEventListener('click', closeMenu);
        }
    });
} 