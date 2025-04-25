import { AfterViewInit, Component, ElementRef, ViewChild } from '@angular/core';

//declaration des variales
const VIDE = 0;
const ARBRE = 1;
const FEU = 2;
const CENDRE = 3;
const SIZE = 100;
const C = 5; 

// Type représentant les différentes valeurs possibles pour une cellule
type Cellule = 0 | 1 | 2 | 3;

@Component({
  selector: 'app-forest',
  template: `
    <div class="controls">
      <button (click)="startTimer()">
        {{ simulationEnCours ? 'Arrêter l incendie' : 'Démarrer l incendie ' }}
      </button>
      <button (click)="refreshForet()">Refresh</button>
    </div>

    <canvas #canvas width="500" height="500" (click)="onCanvasClick($event)"></canvas>
  `,
  styleUrls: ['./forest.component.css']
})
export class ForestComponent implements AfterViewInit {
  @ViewChild('canvas', { static: true }) canvasRef!: ElementRef<HTMLCanvasElement>;
  ctx!: CanvasRenderingContext2D;
  foret: Cellule[][] = [];
  simulationEnCours = false; // Indique si la simulation est en cours
  timerId!: ReturnType<typeof setInterval>;  // Identifiant de l'intervalle pour arrêter/démarrer la simulation

  ngAfterViewInit() {
    this.ctx = this.canvasRef.nativeElement.getContext('2d')!;
    this.initForet();
    this.drawForet();
  }
 
  //Initialise la forêt avec des arbres aléatoires (80%)  
  initForet(): void {
    this.foret = Array.from({ length: SIZE }, () =>
      Array.from({ length: SIZE }, () => (Math.random() < 0.8 ? ARBRE : VIDE))
    );
    this.foret[Math.floor(SIZE / 2)][Math.floor(SIZE / 2)] = FEU; // feu au centre
  }
  
  //Réinitialise complètement la forêt et l'affiche à nouveau.
  refreshForet(): void {
    this.initForet();  // Réinitialise la forêt
    this.drawForet();  // Redessine la forêt
  }
  //Dessine la forêt
  drawForet(): void {
    for (let i = 0; i < SIZE; i++) {
      for (let j = 0; j < SIZE; j++) {
        const x = j * C;
        const y = i * C;
  
        switch (this.foret[i][j]) {
          case VIDE:   this.ctx.fillStyle = 'white'; break;
          case ARBRE:  this.ctx.fillStyle = 'green'; break;
          case FEU:    this.ctx.fillStyle = 'red';   break;
          case CENDRE: this.ctx.fillStyle = 'gray';  break;
        }
  
        this.ctx.fillRect(x, y, C, C);
      }
    }
  }
//propagation du feu 
  calculEtape(): void {
    const copie = this.foret.map(row => [...row]);
  
    for (let i = 0; i < SIZE; i++) {
      for (let j = 0; j < SIZE; j++) {
        switch (copie[i][j]) {
          case ARBRE:
            const v4 = this.voisinage4(copie, i, j);
            const v8 = this.voisinage8(copie, i, j);
            if (v4 > 0 && Math.random() < 0.8) {
              this.foret[i][j] = FEU;
            } else if (v8 > v4 && Math.random() < 0.5) {
              this.foret[i][j] = FEU;
            }
            break;
          case FEU:
            this.foret[i][j] = CENDRE;
            break;
        }
      }
    }
  }

  voisinage4(f: Cellule[][], x: number, y: number): number {
    let feu = 0;
    if (x > 0 && f[x - 1][y] === FEU) feu++;
    if (x < SIZE - 1 && f[x + 1][y] === FEU) feu++;
    if (y > 0 && f[x][y - 1] === FEU) feu++;
    if (y < SIZE - 1 && f[x][y + 1] === FEU) feu++;
    return feu;
  }

  voisinage8(f: Cellule[][], x: number, y: number): number {
    let feu = 0;
    for (let i = -1; i <= 1; i++) {
      for (let j = -1; j <= 1; j++) {
        if (i === 0 && j === 0) continue;
        const xi = x + i, yj = y + j;
        if (xi >= 0 && xi < SIZE && yj >= 0 && yj < SIZE) {
          if (f[xi][yj] === FEU) feu++;
        }
      }
    }
    return feu;
  }

  //Démarre ou arrête la simulation en fonction de l'état actuel.
  startTimer(): void {
    if (this.simulationEnCours) {
      this.simulationEnCours = false;
      clearInterval(this.timerId);
    } else {
      this.simulationEnCours = true;
      this.timerId = setInterval(() => {
        this.calculEtape();
        this.drawForet();
      }, 300);
    }
  }
// si on clique sur un arbre, on l'allume (feu).

  onCanvasClick(event: MouseEvent): void {
    const rect = this.canvasRef.nativeElement.getBoundingClientRect();
    const x = Math.floor((event.clientX - rect.left) / C);
    const y = Math.floor((event.clientY - rect.top) / C);
  
    if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
      if (this.foret[y][x] === ARBRE) {
        this.foret[y][x] = FEU;
        console.log(`Feu allumé à (${x}, ${y})`);
        this.drawForet();
      }
    }
  }
}
