const fs = require('fs');
const readline = require('readline');

const lineReader = readline.createInterface({
    input: fs.createReadStream('index.html')
});

(async () => {
    let rawLines = [];

    for await(const line of lineReader ) {
        rawLines.push(line.trim());
    }

    rawLines = rawLines.filter((line) => !!line);

    const result = rawLines.reduce((acc, curr) => {
        acc += `\ncontent += "${curr.replace(/\\([\s\S])|(")/g,"\\$1$2")}";`;

        return acc;
    }, "String content = \"\";");

    console.log(result);
})();



