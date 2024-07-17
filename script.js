document.addEventListener('DOMContentLoaded', function () {
  const fetchDataBtn = document.getElementById('fetchDataBtn');
  const moistureDataDiv = document.getElementById('moisture-data');

  fetchDataBtn.addEventListener('click', function () {
      fetch('/soil-data')
          .then(response => response.json())
          .then(data => {
              moistureDataDiv.innerHTML = '';
              data.forEach(entry => {
                  const p = document.createElement('p');
                  p.textContent = `Moisture: ${entry.moisture}%`;
                  moistureDataDiv.appendChild(p);
              });
          })
          .catch(error => {
              console.error('Error fetching data:', error);
          });
  });
});
