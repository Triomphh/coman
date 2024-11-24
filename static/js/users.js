function showUserForm() 
{
    document.getElementById('userModal').classList.remove('hidden');
}

function hideUserForm() 
{
    document.getElementById('userModal').classList.add('hidden');
}

function deleteUser(userId) 
{
    if (confirm('Are you sure you want to delete this user?')) 
    {
        fetch(`/users/delete/${userId}`, {
            method: 'DELETE'
        })
        .then(response => {
            if (response.ok) 
                window.location.reload();
            else 
                alert('Failed to delete user');
        })
        .catch(error => {
            console.error('Error:', error);
            alert('Failed to delete user');
        });
    }
}

document.getElementById('userForm').addEventListener('submit', function(e) 
{
    e.preventDefault();
    
    const formData = {
        name:     document.getElementById('name').value,
        email:    document.getElementById('email').value,
        password: document.getElementById('password').value,
        role:     parseInt(document.getElementById('role').value)
    };

    fetch('/users/create', {
        method:  'POST',
        headers: { 'Content-Type': 'application/json' },
        body:    JSON.stringify(formData)
    })
    .then(response => {
        if (response.ok) 
            window.location.reload();
        else
            alert('Failed to create user');
    })
    .catch(error => {
        console.error('Error:', error);
        alert('Failed to create user');
    });
});