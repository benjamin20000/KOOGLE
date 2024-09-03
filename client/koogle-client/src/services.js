// services.js

export function get_urls(word) {
    console.log('word',word)
    return fetch( `http://localhost:5079/words/${encodeURIComponent(word)}`)
        .then(response => {
            if (!response.ok) { // Check if response is ok (status is in the range 200-299)
                throw new Error('Network response was not ok');
            }
            return response.json(); // Parse the JSON from the response
        })
        .catch(error => {
            console.error('There was a problem with the fetch operation:', error);
        });
}
