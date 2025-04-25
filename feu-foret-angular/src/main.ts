import { bootstrapApplication } from '@angular/platform-browser';
import { appConfig } from './app/app.config';
import { ForestComponent } from './app/forest/forest.component';

bootstrapApplication(ForestComponent,appConfig)
  .catch((err) => console.error(err));
