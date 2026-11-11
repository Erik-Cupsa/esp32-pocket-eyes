'use strict';
const data = window.POCKET_FRAMES;
const canvas = document.querySelector('canvas');
const ctx = canvas.getContext('2d');
const byId = id => document.getElementById(id);
let stage = 'eyes', elapsed = 0, last = performance.now();
let paused = matchMedia('(prefers-reduced-motion: reduce)').matches;
function sync() {
  byId('eyes').setAttribute('aria-pressed', stage === 'eyes');
  byId('demo').setAttribute('aria-pressed', stage === 'demo');
  byId('pause').textContent = paused ? 'Play · P' : 'Pause · P';
}
function reset(next = stage) {
  stage = next; elapsed = 0; last = performance.now(); paused = false; sync();
}
function togglePause() { paused = !paused; last = performance.now(); sync(); }
function draw(now) {
  // A click can run after a frame timestamp was sampled but before draw runs.
  // Clamp that tiny negative delta so a restart never indexes frame -1.
  if (!paused) elapsed = (elapsed + Math.max(0, now - last)) % data.duration;
  last = now;
  ctx.fillStyle = '#000'; ctx.fillRect(0, 0, canvas.width, canvas.height);
  const frame = Math.min(data[stage].length - 1, Math.floor(elapsed * data.fps / 1000));
  for (const [x, y, w, h, r, on] of data[stage][frame]) {
    ctx.fillStyle = on ? (byId('warm').checked ? '#ffae47' : '#65e6ef') : '#000';
    ctx.beginPath(); ctx.roundRect(x, y, w, h, r); ctx.fill();
  }
  byId('time').textContent = `${(elapsed / 1000).toFixed(1)} s`;
  byId('scrub').value = elapsed;
  requestAnimationFrame(draw);
}
byId('eyes').onclick = () => reset('eyes');
byId('demo').onclick = () => reset('demo');
byId('reset').onclick = () => reset();
byId('pause').onclick = togglePause;
byId('clean').onclick = () => document.body.classList.toggle('clean');
byId('scrub').oninput = event => { elapsed = Number(event.target.value); paused = true; sync(); };
document.addEventListener('visibilitychange', () => { last = performance.now(); });
document.addEventListener('keydown', event => {
  if (event.ctrlKey || event.metaKey || event.altKey || event.repeat) return;
  if (event.target.matches('input,textarea,select,button') && event.key !== 'Escape') return;
  const key = event.key.toLowerCase();
  const actions = {e:() => reset('eyes'), d:() => reset('demo'), r:() => reset(),
    p:togglePause, f:() => document.body.classList.toggle('clean'),
    escape:() => document.body.classList.remove('clean')};
  if (actions[key]) { event.preventDefault(); actions[key](); }
});
sync(); requestAnimationFrame(draw);
