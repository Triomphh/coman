function showTaskForm() 
{
    document.getElementById('taskModal').classList.remove('hidden');
}

function hideTaskForm() 
{
    document.getElementById('taskModal').classList.add('hidden');
}

document.addEventListener('DOMContentLoaded', () => {
    document.getElementById('taskForm').addEventListener('submit', async (e) => {
        e.preventDefault();
        
        const projectId = document.getElementById('project_id').value;
        const formData = {
            title:       document.getElementById('task_title').value,
            description: document.getElementById('task_description').value,
            priority:    parseInt(document.getElementById('task_priority').value),
            deadline:    document.getElementById('task_deadline').value
        };

        try {
            const response = await fetch(`/projects/${projectId}/tasks/create`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(formData)
            });

            if (response.ok) 
                window.location.reload();
            else 
                alert('Failed to create task');
        } 
        catch (error) 
        {
            console.error('Error:', error);
            alert('Failed to create task');
        }
    });
}); 