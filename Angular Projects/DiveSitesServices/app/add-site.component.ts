import {Component, Input, Output, EventEmitter} from '@angular/core';
import {SiteManagementService} from './site-management.service'
import {Router} from '@angular/router'

@Component({
  selector: 'add-site-view',
  templateUrl: 'app/add-site.template.html',
  styles: [`
	h3{
		font-weight: bold;
		color: maroon;
	}
	`]
})
export class AddSiteComponent {

	siteName: string;
	
	constructor(private siteService: SiteManagementService, 
		private router: Router) { }

	add() {
		this.siteService.addSite({id: 0, name:this.siteName});
		this.router.navigate(['/list']);
	}
}