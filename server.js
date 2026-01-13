const express = require('express');
const path = require('path');
const app = express();
const PORT = 3000;

// Esto le dice a Node que sirva los archivos estáticos (HTML, CSS, IMG)
// desde la carpeta actual
app.use(express.static(path.join(__dirname, '/')));

// Ruta principal para asegurar que cargue el index.html
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.listen(PORT, () => {
    console.log(`🚀 Servidor corriendo en: http://localhost:${PORT}`);
});