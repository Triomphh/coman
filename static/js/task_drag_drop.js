function allowDrop(ev) 
{
    ev.preventDefault();
}

function drag(ev) 
{
    const taskElement = ev.target.closest('[data-task-id]');
    ev.dataTransfer.setData("taskId", taskElement.dataset.taskId);
    ev.dataTransfer.setData("currentStatus", taskElement.dataset.taskStatus);
}

function drop(ev, newStatus) 
{
    ev.preventDefault();
    const taskId = ev.dataTransfer.getData("taskId");
    const currentStatus = ev.dataTransfer.getData("currentStatus");
    
    if (currentStatus === newStatus) return;

    // Get the task data from the DOM
    const taskElement  = document.querySelector(`[data-task-id="${taskId}"]`);
    const title        = taskElement.querySelector('h4').textContent;
    const description  = taskElement.querySelector('p').textContent;
    const prioritySpan = taskElement.querySelector('span');
    const priority     = prioritySpan.textContent.trim();
    const deadline     = taskElement.querySelector('.text-gray-500').textContent;

    // Map priority text to value
    const priorityMap = { 'High': 2, 'Medium': 1, 'Low': 0 };

    // Prepare the update data
    const updateData = {
        title: title,
        description: description,
        priority: priorityMap[priority],
        deadline: deadline,
        status: ['TODO', 'IN_PROGRESS', 'DONE'].indexOf(newStatus)
    };

    // Send update to server using AJAX
    fetch(`/projects/${projectId}/tasks/${taskId}/update`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(updateData)
    })
    .then(response => {
        if (response.ok) {
            // Move the task element to the new column
            const targetColumn = document.querySelector(`[ondrop="drop(event, '${newStatus}')"]`);
            const tasksContainer = targetColumn.querySelector('.space-y-4');
            tasksContainer.appendChild(taskElement);
            
            // Update the task's status attribute
            taskElement.dataset.taskStatus = newStatus;
        }
    })
    .catch(error => {
        console.error('Error:', error);
    });
} 