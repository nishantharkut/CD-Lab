function square(x) {
  return x * x;
}

function runTest() {
  let sum = 0;
  for (let i = 0; i < 1e7; i++) {
    sum += square(i);
  }
  return sum;
}

console.time("Execution Time");
runTest();
console.timeEnd("Execution Time");
