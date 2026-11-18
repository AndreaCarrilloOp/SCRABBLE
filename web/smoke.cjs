const assert = require('node:assert/strict');
const createScrabble = require('../dist/engine.js');
(async () => {
  const engine = await createScrabble();
  const call = (name, types = [], args = []) =>
    JSON.parse(engine.ccall('scrabble_' + name, 'string', types, args));
  const state = () => call('state').state;
  assert.equal(state().board.length, 225);
  const premiums = state().board.reduce((counts, cell) => {
    counts[cell.bonus] = (counts[cell.bonus] || 0) + 1;
    return counts;
  }, {});
  assert.deepEqual(premiums, {'3P':8,'2P':16,'3L':12,'2L':24,'**':1,'':164});
  const start = () => call('start', ['number','string','string','string','string'], [2,'Ana','Luis','','']);
  assert.equal(start().ok, true);
  let before = state();
  assert.equal(before.rack.length, 6);
  assert.equal(before.players.length, 2);
  assert.equal(before.bag, 82);
  const rejected = call('play', ['number','number','number','string'], [0,0,0,'CASA']);
  assert.equal(rejected.ok, false);
  assert.deepEqual(rejected.state, before);
  const invalid = call('exchange', ['string'], ['@']);
  assert.equal(invalid.ok, false);
  assert.deepEqual(invalid.state, before);
  const changed = call('exchange', ['string'], [before.rack[0].letter]);
  assert.equal(changed.ok, true);
  assert.equal(changed.state.active, 1 - before.active);
  assert.equal(changed.state.bag, before.bag);
  for (let i = 0; i < 4; i++) assert.equal(call('pass').ok, true);
  assert.equal(state().finished, true);
  assert.equal(call('pass').ok, false);
  assert.equal(start().ok, true);
  assert.equal(state().finished, false);
  assert.equal(call('finish').ok, true);
  assert.equal(state().finished, true);
  console.log('WASM smoke test passed');
})().catch(error => { console.error(error); process.exitCode = 1; });
