function showProjectForm()
{
    document.getElementById('projectModal').classList.remove('hidden');
}

function hideProjectForm() 
{
    document.getElementById('projectModal').classList.add('hidden');
}

document.addEventListener('DOMContentLoaded', () => 
{
    document.getElementById('projectForm').addEventListener('submit', async (e) => 
    {
        e.preventDefault();
        
        const formData = {
            name: document.getElementById('name').value,
            description: document.getElementById('description').value
        };

        try 
        {
            const response = await fetch('/projects/create', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(formData)
            });

            if (response.ok) 
                window.location.reload();
            else 
                alert('Failed to create project');
        } 
        catch (error) 
        {
            console.error('Error:', error);
            alert('Failed to create project');
        }
    });
}); 