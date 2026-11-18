'use strict';
const $=id=>document.getElementById(id);
const colors={'3P':'p3','2P':'p2','3L':'l3','2L':'l2','**':'center'};
let engine,state,revealed=false;const cells=[];
function status(message,error=false){$('status').textContent=message;$('status').classList.toggle('error',error);}
function call(name,types=[],args=[]){return JSON.parse(engine.ccall('scrabble_'+name,'string',types,args));}
function coord(n){const e=document.createElement('span');e.className='coord';e.textContent=n;$('board').append(e);}
for(let c=0;c<15;c++)coord(c);coord('');
for(let r=0;r<15;r++){for(let c=0;c<15;c++){
 const b=document.createElement('button');b.type='button';b.className='cell';b.append(document.createElement('span'));
 b.onclick=()=>{$('row').value=r;$('col').value=c;highlight();};cells.push(b);$('board').append(b);
}coord(r);}
function highlight(){const r=Number($('row').value),c=Number($('col').value);cells.forEach((b,i)=>b.classList.toggle('selected',!!state?.started&&!state.finished&&r>=0&&r<15&&c>=0&&c<15&&i===r*15+c));}
function names(){const old=[...$('names').querySelectorAll('input')].map(x=>x.value);$('names').replaceChildren();for(let i=0;i<Number($('count').value);i++){const l=document.createElement('label'),x=document.createElement('input');l.textContent='Jugador '+(i+1);x.maxLength=30;x.value=old[i]||'';x.placeholder=l.textContent;l.append(x);$('names').append(l);}}
function render(){
 state.board.forEach((t,i)=>{const b=cells[i];b.className='cell '+(colors[t.bonus]||'')+(t.letter?' occupied':'');b.firstChild.textContent=t.letter;b.setAttribute('aria-label','Fila '+Math.floor(i/15)+', columna '+i%15+', '+(t.letter||t.bonus||'vacía'));});highlight();
 $('setup').hidden=state.started;$('game').hidden=!state.started;if(!state.started)return;
 const top=Math.max(...state.players.map(p=>p.score)),winners=state.players.filter(p=>p.score===top).map(p=>p.name);
 $('turn').textContent=state.finished?(winners.length>1?'Empate: ':'Ganador: ')+winners.join(', '):'Turno de '+state.players[state.active].name;
 $('bag').textContent='Bolsa: '+state.bag+' fichas · Pases: '+state.passes;
 $('scores').replaceChildren();state.players.forEach((p,i)=>{const d=document.createElement('div');d.className='score'+(!state.finished&&i===state.active?' active':'');const n=document.createElement('span'),s=document.createElement('span');n.textContent=p.name;s.textContent=p.score+' pts';d.append(n,s);$('scores').append(d);});
 $('history').textContent=state.history||'Sin jugadas.';$('handoff').hidden=state.finished||revealed;$('controls').hidden=state.finished||!revealed;$('finish').hidden=state.finished;$('restart').hidden=!state.finished;
 $('rack').replaceChildren();if(revealed&&!state.finished)state.rack.forEach(t=>{const b=document.createElement('button');b.type='button';b.className='tile';b.textContent=t.letter;const p=document.createElement('small');p.textContent=t.points;b.append(p);b.onclick=()=>{if($('word').value.length<6)$('word').value+=t.letter;$('word').focus();};$('rack').append(b);});
}
function action(name,types=[],args=[]){try{const result=call(name,types,args);state=result.state;if(result.ok&&result.advanced){revealed=false;$('word').value='';$('letters').value='';}render();status(result.message,!result.ok);}catch(e){status('Error del motor: '+e.message,true);}}
names();$('count').onchange=names;$('row').oninput=highlight;$('col').oninput=highlight;
$('start').onsubmit=e=>{e.preventDefault();const n=[...$('names').querySelectorAll('input')].map(x=>x.value.trim());while(n.length<4)n.push('');action('start',['number','string','string','string','string'],[Number($('count').value),...n]);};
$('reveal').onclick=()=>{revealed=true;render();};
$('play').onsubmit=e=>{e.preventDefault();action('play',['number','number','number','string'],[Number($('row').value),Number($('col').value),Number($('direction').value),$('word').value.toUpperCase()]);};
$('exchange').onsubmit=e=>{e.preventDefault();action('exchange',['string'],[$('letters').value.toUpperCase()]);};
$('pass').onclick=()=>action('pass');$('finish').onclick=()=>{if(confirm('¿Finalizar partida?'))action('finish');};
$('restart').onclick=()=>{$('game').hidden=true;$('setup').hidden=false;revealed=false;};
(async()=>{try{engine=await createScrabble();state=call('state').state;render();$('startButton').disabled=false;status('Motor C++ listo. Inicia una partida.');}catch(e){status('No se pudo cargar WebAssembly: '+e.message,true);}})();
