import {Component, Input, Output, EventEmitter} from '@angular/core';
import {Router, ActivatedRoute} from '@angular/router';

import {SiteManagementService} from './site-management.service'

@Component({
  selector: 'edit-site-view',
  templateUrl: 'app/edit-site.template.html'
})

export class EditSiteComponent {
	siteId: number;
	siteName: string;
	private parSub: any;
	
	constructor(private siteService: SiteManagementService,
		private route: ActivatedRoute,
		private router: Router) {
		this.siteId = this.route.snapshot.params['id'];
		this.siteName = this.siteService.getSiteById(this.siteId).name;
	}

	save() {
		this.siteService.saveSite({id: this.siteId, name:this.siteName});
		this.router.navigate(['/list']);
	}
}