window.onload = function () {
    const canvas = document.getElementById("canvas"),
        context = canvas.getContext("2d"),
        width = (canvas.width = window.innerWidth),
        height = (canvas.height = window.innerHeight);

    const scale = Math.min(width, height) / 15;
    let isAnimating = true;
    

    const colors = {
        memory: "#333333",      // Dark gray for memory blocks
        variable: "#00ff00",    // Green for variables
        pointer: "#ff0000",     // Red for pointers
        arrow: "#0088ff",       // Blue for arrows
        text: "#ffffff",        // White for text
        highlight: "#ffff00"    // Yellow for highlights
    };

    // Memory layout simulation
    const memory = {
        i: { address: 0x1000, value: 42 },
        pi: { address: 0x2000, value: 0x1000 },  // Points to i
        ppi: { address: 0x3000, value: 0x2000 }, // Points to pi
        null: { address: 0x0000, value: 0x0000 }
    };

    // Center coordinate system
    context.translate(width / 2, height / 2);

    function drawMemoryBlock(x, y, width, height, label, value, address, type = "variable") {
        // Draw memory block
        context.beginPath();
        context.rect(x, y, width, height);
        context.strokeStyle = colors.memory;
        context.lineWidth = 2;
        context.stroke();

        // Fill based on type
        context.fillStyle = type === "variable" ? colors.variable : colors.pointer;
        context.fillRect(x, y, width, height);

        // Add labels
        context.fillStyle = colors.text;
        context.font = '16px monospace';
        context.textAlign = 'left';
        context.textBaseline = 'middle';

        // Variable/pointer name
        context.fillText(label, x + 10, y + height/4);

        // Value
        let valueText = type === "variable" ?
            value.toString() :
            (value === 0 ? "NULL" : "0x" + value.toString(16));
        context.fillText(valueText, x + 10, y + height/2);

        // Memory address
        context.fillStyle = colors.text;
        context.fillText("0x" + address.toString(16), x + 10, y + 3*height/4);
    }

    function drawArrow(fromX, fromY, toX, toY, color = colors.arrow) {
        const headLen = 10;
        const angle = Math.atan2(toY - fromY, toX - fromX);

        // Draw line
        context.beginPath();
        context.strokeStyle = color;
        context.lineWidth = 2;
        context.moveTo(fromX, fromY);
        context.lineTo(toX, toY);
        context.stroke();

        // Draw arrowhead
        context.beginPath();
        context.moveTo(toX, toY);
        context.lineTo(toX - headLen * Math.cos(angle - Math.PI/6),
                      toY - headLen * Math.sin(angle - Math.PI/6));
        context.moveTo(toX, toY);
        context.lineTo(toX - headLen * Math.cos(angle + Math.PI/6),
                      toY - headLen * Math.sin(angle + Math.PI/6));
        context.stroke();
    }

    function drawExpressionValue(x, y, expression, value) {
        context.fillStyle = colors.text;
        context.font = '18px monospace';
        context.textAlign = 'left';
        context.fillText(`${expression} = ${value}`, x, y);
    }

    function drawPointerExplanations() {
        const expressions = [
            { expr: "i", value: memory.i.value },
            { expr: "&i", value: "0x" + memory.i.address.toString(16) },
            { expr: "pi", value: "0x" + memory.pi.value.toString(16) },
            { expr: "*pi", value: memory.i.value },
            { expr: "&pi", value: "0x" + memory.pi.address.toString(16) },
            { expr: "ppi", value: "0x" + memory.ppi.value.toString(16) },
            { expr: "*ppi", value: "0x" + memory.pi.value.toString(16) },
            { expr: "**ppi", value: memory.i.value }
        ];

        let y = -height/3;
        expressions.forEach(({expr, value}) => {
            drawExpressionValue(-width/3, y, expr, value);
            y += 30;
        });
    }

    function drawMemoryDiagram() {
        const blockWidth = 150;
        const blockHeight = 80;
        const spacing = 40;

        // Draw variable i
        drawMemoryBlock(-blockWidth*1.5, -blockHeight/2,
                       blockWidth, blockHeight,
                       "i", memory.i.value, memory.i.address, "variable");

        // Draw pointer pi
        drawMemoryBlock(0, -blockHeight/2,
                       blockWidth, blockHeight,
                       "pi", memory.pi.value, memory.pi.address, "pointer");

        // Draw pointer to pointer ppi
        drawMemoryBlock(blockWidth*1.5, -blockHeight/2,
                       blockWidth, blockHeight,
                       "ppi", memory.ppi.value, memory.ppi.address, "pointer");

        // Draw arrows
        // ppi -> pi
        drawArrow(blockWidth*1.5, 0,
                 blockWidth/2, 0);

        // pi -> i
        drawArrow(0, 0,
                 -blockWidth, 0);

        // Draw null pointer example
        drawMemoryBlock(0, blockHeight*1.5,
                       blockWidth, blockHeight,
                       "null_ptr", 0x0000, 0x4000, "pointer");
    }

    function drawTitle() {
        context.fillStyle = colors.text;
        context.font = 'bold 24px Arial';
        context.textAlign = 'center';
        context.fillText("C 언어의 포인터 시각화", 0, -height/2 + 30);
    }

    function drawLegend() {
        const x = width/2 - 200;
        let y = -height/2 + 60;

        context.font = '16px Arial';
        context.textAlign = 'left';

        // Variable
        context.fillStyle = colors.variable;
        context.fillRect(x, y, 20, 20);
        context.fillStyle = colors.text;
        context.fillText("변수", x + 30, y + 15);

        // Pointer
        y += 30;
        context.fillStyle = colors.pointer;
        context.fillRect(x, y, 20, 20);
        context.fillStyle = colors.text;
        context.fillText("포인터", x + 30, y + 15);

        // Reference
        y += 30;
        context.fillStyle = colors.arrow;
        context.fillRect(x, y, 20, 20);
        context.fillStyle = colors.text;
        context.fillText("참조", x + 30, y + 15);
    }

    function draw() {
        // Clear canvas
        context.fillStyle = 'black';
        context.fillRect(-width/2, -height/2, width, height);

        drawTitle();
        drawLegend();
        drawMemoryDiagram();
        drawPointerExplanations();

        // Add explanatory text
        context.fillStyle = colors.text;
        context.font = '16px Arial';
        context.textAlign = 'left';
        context.fillText("포인터 연산자:", -width/2 + 20, height/2 - 80);
        context.fillText("* : 역참조 연산자 (포인터가 가리키는 값을 얻음)", -width/2 + 40, height/2 - 50);
        context.fillText("& : 주소 연산자 (변수의 메모리 주소를 얻음)", -width/2 + 40, height/2 - 20);
    }

    function animate() {
        if (isAnimating) {
            draw();
            requestAnimationFrame(animate);
        }
    }

    animate();
};
