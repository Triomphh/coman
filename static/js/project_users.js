let selectedUserId = null;

function showAddUserModal() 
{
    document.getElementById('addUserModal').classList.remove('hidden');
    fetchAndDisplayUsers();
}

function hideAddUserModal() 
{
    document.getElementById('addUserModal').classList.add('hidden');
    document.getElementById('user_search').value = '';
    document.getElementById('user_list').innerHTML = '';
    selectedUserId = null;
}

async function fetchAndDisplayUsers() 
{
    try 
    {
        // Fetch both all users and project users
        const [allUsersResponse, projectUsersResponse] = await Promise.all([
            fetch('/users/json'),
            fetch(`/projects/${projectId}/users`)
        ]);

        if (!allUsersResponse.ok || !projectUsersResponse.ok) 
        {
            throw new Error('Failed to fetch users');
        }

        const allUsersData = await allUsersResponse.json();
        const projectUsersData = await projectUsersResponse.json();

        // Create a set of user IDs already in the project
        const projectUserIds = new Set(projectUsersData.users.map(user => user.id));

        // Filter and display users
        const availableUsers = allUsersData.users.map(user => ({
            ...user,
            isInProject: projectUserIds.has(user.id)
        }));

        // Sort users - available users then others
        availableUsers.sort((a, b) => {
            if (a.isInProject !== b.isInProject) 
            {
                return a.isInProject ? 1 : -1;
            }
            return a.name.localeCompare(b.name);
        });

        displayUsers(availableUsers);
    } 
    catch (error) 
    {
        console.error('Error:', error);
        alert('Failed to fetch users');
    }
}

function displayUsers(users) 
{
    const userList = document.getElementById('user_list');
    userList.innerHTML = '';

    users.forEach(user => {
        const userElement = document.createElement('div');
        const isDisabled = user.isInProject;
        
        userElement.className = `flex items-center p-3 rounded-md ${ isDisabled ? 'opacity-50 cursor-not-allowed bg-gray-50' 
                                                                                : 'hover:bg-gray-50 cursor-pointer'}`;

        if (!isDisabled) {
            userElement.onclick = () => selectUser(user.id, userElement);
        }
        
        userElement.innerHTML = `
            <img src="/static/profile_pictures/${user.profile_picture}" 
                 alt="${user.name}'s avatar" 
                 class="w-10 h-10 rounded-full object-cover mr-3">
            <div>
                <div class="font-medium">${user.name}</div>
                <div class="text-sm text-gray-500">${user.email}</div>
                ${isDisabled ? '<div class="text-xs text-blue-600">Already in project</div>' : ''}
            </div>
        `;
        
        userList.appendChild(userElement);
    });
}

function selectUser(userId, element) 
{
    // Remove previous selection
    const previousSelected = document.querySelector('.bg-blue-50');
    if (previousSelected) 
    {
        previousSelected.classList.remove('bg-blue-50');
    }
    
    // Add selection to clicked element
    element.classList.add('bg-blue-50');
    selectedUserId = userId;
}

async function addSelectedUserToProject() 
{
    if (!selectedUserId) 
    {
        alert('Please select a user first');
        return;
    }

    try 
    {
        const response = await fetch(`/projects/${projectId}/users/add`, {
            method:  'POST',
            headers: { 'Content-Type': 'application/json' },
            body:    JSON.stringify({ user_id: selectedUserId })
        });

        if (response.ok) 
        {
            window.location.reload();
        } 
        else 
        {
            throw new Error('Failed to add user to project');
        }
    } 
    catch (error) 
    {
        console.error('Error:', error);
        alert('Failed to add user to project');
    }
}

// Add search functionality
document.getElementById('user_search').addEventListener('input', function(e) 
{
    const searchTerm = e.target.value.toLowerCase();
    const userElements = document.getElementById('user_list').children;
    
    Array.from(userElements).forEach(element => {
        const userName = element.querySelector('.font-medium').textContent.toLowerCase();
        const userEmail = element.querySelector('.text-gray-500').textContent.toLowerCase();
        
        if (userName.includes(searchTerm) || userEmail.includes(searchTerm)) 
        {
            element.style.display = '';
        } 
        else 
        {
            element.style.display = 'none';
        }
    });
}); 