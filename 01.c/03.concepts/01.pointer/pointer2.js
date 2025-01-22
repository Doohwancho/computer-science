window.onload = function () {
    const canvas = document.getElementById("canvas"),
        context = canvas.getContext("2d"),
        width = (canvas.width = window.innerWidth),
        height = (canvas.height = window.innerHeight);

    const BLOCK_WIDTH = 80;
    const BLOCK_HEIGHT = 50;
    let currentView = "1d";
    let isAnimating = true;
    let highlightIndex = 0;
    
    const colors = {
        memory: "#333333",
        value: "#00ff00",
        pointer: "#ff0000",
        arrow: "#0088ff",
        text: "#ffffff",
        highlight: "#ffff00"
    };

    // Sample arrays with base addresses
    const arrays = {
        "1d": {
            data: [1, 2, 3, 4, 5],
            baseAddress: 0x1000,
            name: "int arr[5]"
        },
        "2d": {
            data: [
                [1, 2, 3],
                [4, 5, 6],
                [7, 8, 9]
            ],
            baseAddress: 0x2000,
            name: "int arr[3][3]"
        },
        "3d": {
            data: [
                [[1, 2], [3, 4]],
                [[5, 6], [7, 8]]
            ],
            baseAddress: 0x3000,
            name: "int arr[2][2][2]"
        }
    };

    function drawBlock(x, y, value, address, highlight = false, label = "") {
        // Draw memory block
        context.beginPath();
        context.rect(x, y, BLOCK_WIDTH, BLOCK_HEIGHT);
        context.strokeStyle = highlight ? colors.highlight : colors.memory;
        context.lineWidth = 2;
        context.stroke();

        // Fill with semi-transparent color
        context.fillStyle = highlight ? colors.highlight + '40' : colors.value + '40';
        context.fillRect(x, y, BLOCK_WIDTH, BLOCK_HEIGHT);

        // Draw value and address
        context.fillStyle = colors.text;
        context.font = '14px monospace';
        context.textAlign = 'center';
        context.fillText(value.toString(), x + BLOCK_WIDTH/2, y + BLOCK_HEIGHT/3);
        context.fillText('0x' + address.toString(16), x + BLOCK_WIDTH/2, y + 2*BLOCK_HEIGHT/3);

        // Draw label if provided
        if (label) {
            context.fillStyle = colors.text;
            context.textAlign = 'left';
            context.fillText(label, x, y - 5);
        }
    }

    function drawArrow(fromX, fromY, toX, toY) {
        const headLength = 10;
        const angle = Math.atan2(toY - fromY, toX - fromX);

        // Draw line
        context.beginPath();
        context.strokeStyle = colors.arrow;
        context.lineWidth = 2;
        context.moveTo(fromX, fromY);
        context.lineTo(toX, toY);
        context.stroke();

        // Draw arrowhead
        context.beginPath();
        context.moveTo(toX, toY);
        context.lineTo(
            toX - headLength * Math.cos(angle - Math.PI/6),
            toY - headLength * Math.sin(angle - Math.PI/6)
        );
        context.lineTo(
            toX - headLength * Math.cos(angle + Math.PI/6),
            toY - headLength * Math.sin(angle + Math.PI/6)
        );
        context.fillStyle = colors.arrow;
        context.fill();
    }

    function draw1DArray() {
        const arr = arrays["1d"];
        // Center horizontally: (width - total array width) / 2
        const startX = (width - (arr.data.length * (BLOCK_WIDTH + 10))) / 2;
        // Center vertically
        const startY = height * 0.3;  // Start at 30% from top


        // Draw title
        context.fillStyle = colors.text;
        context.font = 'bold 20px Arial';
        context.textAlign = 'left';
        context.fillText("1D Array: " + arr.name, startX, 50);

        // Draw array blocks
        arr.data.forEach((value, i) => {
            drawBlock(
                startX + i * (BLOCK_WIDTH + 10),
                startY,
                value,
                arr.baseAddress + i * 4,
                i === highlightIndex % arr.data.length,
                i === 0 ? "arr" : ""
            );
        });

        // Draw pointer expressions
        let textY = startY + BLOCK_HEIGHT + 50;
        const expressions = [
            { expr: "arr", value: "0x" + arr.baseAddress.toString(16) },
            { expr: "arr[2]", value: arr.data[2] },
            { expr: "*(arr+2)", value: arr.data[2] },
            { expr: "&arr[2]", value: "0x" + (arr.baseAddress + 8).toString(16) }
        ];

        expressions.forEach(({expr, value}) => {
            context.fillStyle = colors.text;
            context.font = '16px Arial';
            context.textAlign = 'left';
            context.fillText(`${expr} = ${value}`, startX, textY);
            textY += 25;
        });
    }

    function draw2DArray() {
        const arr = arrays["2d"];
        const rows = arr.data.length;
        const cols = arr.data[0].length;
        // Center horizontally
        const startX = (width - (cols * (BLOCK_WIDTH + 10))) / 2;
        // Center vertically
        const startY = height * 0.3;

        // Draw title
        context.fillStyle = colors.text;
        context.font = 'bold 20px Arial';
        context.textAlign = 'left';
        context.fillText("2D Array: " + arr.name, startX, 50);

        // Draw array blocks
        for(let i = 0; i < rows; i++) {
            for(let j = 0; j < cols; j++) {
                drawBlock(
                    startX + j * (BLOCK_WIDTH + 10),
                    startY + i * (BLOCK_HEIGHT + 10),
                    arr.data[i][j],
                    arr.baseAddress + (i * cols + j) * 4,
                    i === Math.floor(highlightIndex/cols) % rows &&
                    j === highlightIndex % cols,
                    i === 0 && j === 0 ? "arr" : ""
                );
            }
            // Draw row pointer
            drawArrow(
                startX - 50,
                startY + i * (BLOCK_HEIGHT + 10) + BLOCK_HEIGHT/2,
                startX,
                startY + i * (BLOCK_HEIGHT + 10) + BLOCK_HEIGHT/2
            );
            context.fillText(
                `arr[${i}]`,
                startX - 90,
                startY + i * (BLOCK_HEIGHT + 10) + BLOCK_HEIGHT/2
            );
        }

        // Draw pointer expressions
        let textY = startY + rows * (BLOCK_HEIGHT + 10) + 50;
        const expressions = [
            { expr: "arr", value: "0x" + arr.baseAddress.toString(16) },
            { expr: "arr[1]", value: "0x" + (arr.baseAddress + cols * 4).toString(16) },
            { expr: "arr[1][2]", value: arr.data[1][2] },
            { expr: "*(*(arr+1)+2)", value: arr.data[1][2] }
        ];

        expressions.forEach(({expr, value}) => {
            context.fillStyle = colors.text;
            context.font = '16px Arial';
            context.textAlign = 'left';
            context.fillText(`${expr} = ${value}`, startX, textY);
            textY += 25;
        });
    }

    function draw3DArray() {
        const arr = arrays["3d"];
        const depth = arr.data.length;
        const rows = arr.data[0].length;
        const cols = arr.data[0][0].length;
        // Center horizontally, accounting for 3D offset
        const startX = (width - (cols * (BLOCK_WIDTH + 10) + depth * 30)) / 2;
        // Center vertically
        const startY = height * 0.3;

        // Draw title
        context.fillStyle = colors.text;
        context.font = 'bold 20px Arial';
        context.textAlign = 'left';
        context.fillText("3D Array: " + arr.name, startX, 50);

        // Draw array blocks with offset for 3D effect
        for(let i = 0; i < depth; i++) {
            const offsetX = i * 30;
            const offsetY = i * 30;
            
            for(let j = 0; j < rows; j++) {
                for(let k = 0; k < cols; k++) {
                    drawBlock(
                        startX + k * (BLOCK_WIDTH + 10) + offsetX,
                        startY + j * (BLOCK_HEIGHT + 10) + offsetY,
                        arr.data[i][j][k],
                        arr.baseAddress + (i * rows * cols + j * cols + k) * 4,
                        i === Math.floor(highlightIndex/(rows*cols)) % depth &&
                        j === Math.floor(highlightIndex/cols) % rows &&
                        k === highlightIndex % cols
                    );
                }
                // Draw plane pointers
                drawArrow(
                    startX - 50 + offsetX,
                    startY + j * (BLOCK_HEIGHT + 10) + offsetY + BLOCK_HEIGHT/2,
                    startX + offsetX,
                    startY + j * (BLOCK_HEIGHT + 10) + offsetY + BLOCK_HEIGHT/2
                );
            }
            // Draw depth pointers
            drawArrow(
                startX - 100,
                startY + offsetY,
                startX - 50 + offsetX,
                startY + offsetY
            );
            context.fillText(
                `arr[${i}]`,
                startX - 140,
                startY + offsetY
            );
        }

        // Draw expressions
        let textY = startY + depth * (BLOCK_HEIGHT + 40) + 50;
        const expressions = [
            { expr: "arr", value: "0x" + arr.baseAddress.toString(16) },
            { expr: "arr[1]", value: "0x" + (arr.baseAddress + rows * cols * 4).toString(16) },
            { expr: "arr[1][0][1]", value: arr.data[1][0][1] },
            { expr: "*(*(*(arr+1)+0)+1)", value: arr.data[1][0][1] }
        ];

        expressions.forEach(({expr, value}) => {
            context.fillStyle = colors.text;
            context.font = '16px Arial';
            context.textAlign = 'left';
            context.fillText(`${expr} = ${value}`, startX, textY);
            textY += 25;
        });
    }

    function drawInstructions() {
        const startY = height - 100;
        context.fillStyle = colors.text;
        context.font = '16px Arial';
        context.textAlign = 'left';
        
        // Center the instructions horizontally
        const startX = width / 2 - 150;
        
        context.fillText("Controls:", startX, startY);
        context.fillText("Space: Pause/Resume", startX, startY + 25);
        context.fillText("Ctrl + 1/2/3: Switch Array Dimension", startX, startY + 50);
    }

    function draw() {
        // Clear canvas
        context.fillStyle = 'black';
        context.fillRect(0, 0, width, height);

        switch(currentView) {
            case "1d": draw1DArray(); break;
            case "2d": draw2DArray(); break;
            case "3d": draw3DArray(); break;
        }

        drawInstructions();
    }

    function animate() {
        if (isAnimating) {
            highlightIndex++;
            draw();
            requestAnimationFrame(animate);
        }
    }

    // Modify the event listener part
    document.addEventListener('keydown', function(event) {
        console.log("Key pressed:", event.key); // Add debug logging
        switch(event.key) { // Changed from event.code to event.key
            case ' ':
                isAnimating = !isAnimating;
                if(isAnimating) animate();
                break;
            case '1':
                currentView = "1d";
                highlightIndex = 0;
                if (!isAnimating) draw();
                break;
            case '2':
                currentView = "2d";
                highlightIndex = 0;
                if (!isAnimating) draw();
                break;
            case '3':
                currentView = "3d";
                highlightIndex = 0;
                if (!isAnimating) draw();
                break;
        }
    });

    draw();
};