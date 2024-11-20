function showTaskForm() 
{
    document.getElementById('taskModal').classList.remove('hidden');
}

function hideTaskForm() 
{
    document.getElementById('taskModal').classList.add('hidden');
}

function showEditTaskForm(taskId, title, description, priority, deadline, status) 
{
    document.getElementById('edit_task_id').value          = taskId;
    document.getElementById('edit_task_title').value       = title;
    document.getElementById('edit_task_description').value = description;
    document.getElementById('edit_task_priority').value    = convertPriorityToValue(priority);
    document.getElementById('edit_task_deadline').value    = deadline;
    document.getElementById('edit_task_status').value      = status;
    document.getElementById('editTaskModal').classList.remove('hidden');
}

function hideEditTaskForm() 
{
    document.getElementById('editTaskModal').classList.add('hidden');
}

function convertPriorityToValue(priority) 
{
    switch(priority.toLowerCase()) 
    {
        case 'high':   return 2;
        case 'medium': return 1;
        case 'low':    return 0;
        default:       return 1;
    }
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

    document.getElementById('editTaskForm').addEventListener('submit', async (e) => {
        e.preventDefault();
        
        const projectId = document.getElementById('project_id').value;
        const taskId = document.getElementById('edit_task_id').value;
        
        const formData = {
            title:       document.getElementById('edit_task_title').value,
            description: document.getElementById('edit_task_description').value,
            priority:    parseInt(document.getElementById('edit_task_priority').value),
            deadline:    document.getElementById('edit_task_deadline').value,
            status:      parseInt(document.getElementById('edit_task_status').value)
        };

        try {
            const response = await fetch(`/projects/${projectId}/tasks/${taskId}/update`, {
                method: 'PUT',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(formData)
            });

            if (response.ok) 
                window.location.reload();
            else 
                alert('Failed to update task');
        } 
        catch (error) 
        {
            console.error('Error:', error);
            alert('Failed to update task');
        }
    });
}); 

async function deleteTask(taskId) 
{
    if (!confirm('Are you sure you want to delete this task?')) 
        return;

    const projectId = document.getElementById('project_id').value;
    
    try 
    {
        const response = await fetch(`/projects/${projectId}/tasks/${taskId}/delete`, {
            method: 'DELETE',
            headers: { 'Content-Type': 'application/json' }
        });

        if (response.ok) 
            window.location.reload();
        else 
            alert('Failed to delete task');
    } 
    catch (error) 
    {
        console.error('Error:', error);
        alert('Failed to delete task');
    }
} 