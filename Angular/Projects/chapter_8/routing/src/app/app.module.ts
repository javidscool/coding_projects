import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';
import { RouterModule, Routes } from '@angular/router';

import {
	routes as childRoutes,
	ProductsModule
} from './products/products.module';

import { AppComponent } from './app.component';
import { HomeComponent } from './home/home.component';
import { AboutComponent } from './about/about.component';
import { ContactComponent } from './contact/contact.component';
import { LoginComponent } from './login/login.component';
import { ProtectedComponent } from './protected/protected.component';
import { ProductsComponent } from './products/products/products.component';

import { AUTH_PROVIDERS } from './auth.service';
import { LoggedInGuard } from './logged-in.guard';


const routes: Routes = [
	// basic routes
	{ path: '', redirectTo: 'home', pathMatch: 'full' },
	{ path: 'home', component: HomeComponent },
	{ path: 'about', component: AboutComponent },
	{ path: 'contact', component: ContactComponent },
	{ path: 'contactus', redirectTo: 'contact' },

	
	// authentication demo
	{ path: 'login', component: LoginComponent },
	{
		path: 'protected',
		component: ProtectedComponent,
		canActivate: [ LoggedInGuard ]
	},

	// nested route
	{
		path: 'products',
		component: ProductsComponent,
		children: childRoutes
	}
];

@NgModule({
	declarations: [
		AppComponent,
		HomeComponent,
		AboutComponent,
		ContactComponent,
		LoginComponent,
		ProtectedComponent
	],
	imports: [
		BrowserModule,
		FormsModule,
		HttpModule,
		RouterModule.forRoot(routes), // <-- routes
		
		// added this for our child module
		ProductsModule
	],
	providers: [
		// uncomment this for "hash-bang" routing
		// note that you need to import LocationStrategy and HashLocationStrategy for this to work
		//{ provide: LocationStrategy, useClass: HashLocationStrategy }
		AUTH_PROVIDERS,
		LoggedInGuard
	],
	bootstrap: [AppComponent]
})
export class AppModule { }
