function toggleMobileSearch() 
{
    const overlay = document.getElementById('mobileSearchOverlay');
    overlay.classList.toggle('hidden');
    
    if (!overlay.classList.contains('hidden')) 
    {
        overlay.querySelector('input').focus();
    }
}

function showProjectForm()
{
    document.getElementById('projectModal').classList.remove('hidden');
}

function hideProjectForm() 
{
    document.getElementById('projectModal').classList.add('hidden');
}

async function deleteProject(projectId) 
{
    if (!confirm('Are you sure you want to delete this project?')) 
        return;

    try 
    {
        const response = await fetch(`/projects/delete/${projectId}`, {
            method: 'DELETE',
            headers: { 'Content-Type': 'application/json' }
        });

        if (response.ok) 
            window.location.reload();
        else 
            alert('Failed to delete project');
    } 
    catch (error) 
    {
        console.error('Error:', error);
        alert('Failed to delete project');
    }
}

function showEditProjectForm(id, name, description, startDate, endDate) 
{
    document.getElementById('edit_project_id').value = id;
    document.getElementById('edit_name').value = name;
    document.getElementById('edit_description').value = description;
    document.getElementById('edit_start_date').value = startDate;
    document.getElementById('edit_end_date').value = endDate;
    document.getElementById('editProjectModal').classList.remove('hidden');
}

function hideEditProjectForm() 
{
    document.getElementById('editProjectModal').classList.add('hidden');
}

document.addEventListener('DOMContentLoaded', () => 
{
    document.getElementById('projectForm').addEventListener('submit', async (e) => 
    {
        e.preventDefault();
        
        const formData = {
            name:        document.getElementById('name').value,
            description: document.getElementById('description').value,
            start_date:  document.getElementById('start_date').value,
            end_date:    document.getElementById('end_date').value
        };

        try 
        {
            const response = await fetch('/projects/create', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
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

    document.getElementById('editProjectForm').addEventListener('submit', async (e) => {
        e.preventDefault();
        
        const projectId = document.getElementById('edit_project_id').value;
        const formData = {
            name: document.getElementById('edit_name').value,
            description: document.getElementById('edit_description').value,
            start_date: document.getElementById('edit_start_date').value,
            end_date: document.getElementById('edit_end_date').value
        };

        try {
            const response = await fetch(`/projects/${projectId}/update`, {
                method: 'PUT',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(formData)
            });

            if (response.ok) {
                window.location.reload();
            } else {
                alert('Failed to update project');
            }
        } catch (error) {
            console.error('Error:', error);
            alert('Failed to update project');
        }
    });
}); 