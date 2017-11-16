import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { RouterModule, ActivatedRoute, Router, Routes } from '@angular/router';

import { MainComponent } from './main/main.component';
import { MoreInfoComponent } from './more-info/more-info.component';
import { ProductComponent } from './product/product.component';
import { ProductsComponent } from './products/products.component';

export const routes: Routes = [
	{ path: '', redirectTo: 'main', pathMatch: 'full' },
	{ path: 'main', component: MainComponent },
	{ path: 'more-info', component: MoreInfoComponent },
	{ path: ':id', component: ProductComponent },
];

@NgModule({
	imports: [
		CommonModule,
		RouterModule
	],
	declarations: [
		MainComponent, 
		MoreInfoComponent, 
		ProductComponent, 
		ProductsComponent
	]
})
export class ProductsModule { }
