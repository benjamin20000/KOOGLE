import React, { useState } from "react";
import ReactDOM from "react-dom/client";
import './index.css'; 
import "./services.js";
import { get_urls } from "./services.js";

// Get the root element
const root = ReactDOM.createRoot(document.getElementById('root'));

const App = () => {
  const [inputValue, setInputValue] = useState(''); // State to store input value
  const [urls, set_urls] = useState([]);
  const [urlsFound, setUrlsFound] = useState(0); // 0: empty result, 1: found, 2: not found

  // Event handler to update state when input changes
  const handleInputChange = (event) => {
    setInputValue(event.target.value);
  };

  // Event handler to save input value on Enter key press
  const handleKeyDown = async (event) => {
    if (event.key === 'Enter') {
      const data = await get_urls(event.target.value); // Fetch the data and wait for the result
      setInputValue(""); // Clear the input value
      console.log("data", data); // Log the data to the console
      if (data === undefined) {
        setUrlsFound(2); // No data found
      } else {
        setUrlsFound(1); // Data found
        set_urls(data);
      }
    }
  };

  const DisplayResult = ({ urls }) => {
    if (urlsFound === 1) { // URLs found
      return (
        <ul>
          {urls.map((url, index) => (
            <li key={index}>
              <a href={url} target="_blank" rel="noopener noreferrer">
                {url}
              </a>
            </li>
          ))}
        </ul>
      );
    } else if (urlsFound === 2) { // URLs not found
      return <p>URLs not found</p>;
    } else { // Initial state or no input
      return <p>Please enter a search term.</p>;
    }
  };

  return (
    <div className="center-container">
      <p>KOOGLE</p>
      <input 
        type="text" 
        value={inputValue} 
        onChange={handleInputChange} 
        onKeyDown={handleKeyDown} // Handle key down event
        placeholder="Enter something..."
      />
      <DisplayResult urls={urls} />
    </div>
  );
};

// Render the App component
root.render(<App />);
