// Reproduce the click/requestAnimationFrame ordering that previously stopped playback.
const assert = require('node:assert/strict');
const fs = require('node:fs');
const vm = require('node:vm');
let now = 1000, nextFrame, rectangles = 0;
const elements = new Map();
const getElement = id => {
  if (!elements.has(id)) elements.set(id, {setAttribute() {}, classList:{toggle() {},remove() {}}, checked:true});
  return elements.get(id);
};
const canvas = {width:466,height:466,getContext:()=>({
  fillRect() {}, beginPath() {}, roundRect() { rectangles++; }, fill() {}
})};
const context = vm.createContext({window:{},console,performance:{now:()=>now},
  matchMedia:()=>({matches:false}),requestAnimationFrame:fn=>{ nextFrame=fn; },
  document:{querySelector:()=>canvas,getElementById:getElement,
    addEventListener() {},body:getElement('body')}});
vm.runInContext(fs.readFileSync('preview/frames.js','utf8'),context);
vm.runInContext(fs.readFileSync('preview/preview.js','utf8'),context);
nextFrame(1000);
now=1100;
getElement('demo').onclick();
rectangles=0;
nextFrame(1099); // Frame timestamp can predate the latest click timestamp.
assert.equal(rectangles,3, 'Demo must still render after a near-simultaneous restart');
getElement('eyes').onclick();
nextFrame(1099);
getElement('scrub').oninput({target:{value:'9500'}});
nextFrame(1200);
assert.equal(getElement('time').textContent,'9.5 s');
assert.equal(getElement('pause').textContent,'Play · P');
nextFrame(2000);
assert.equal(getElement('time').textContent,'9.5 s', 'Paused pose stays fixed');
console.log('PASS: preview reset race, stage rendering, scrub and pause');
