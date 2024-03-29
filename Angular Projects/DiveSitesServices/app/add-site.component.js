"use strict";
var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
var __metadata = (this && this.__metadata) || function (k, v) {
    if (typeof Reflect === "object" && typeof Reflect.metadata === "function") return Reflect.metadata(k, v);
};
var core_1 = require('@angular/core');
var site_management_service_1 = require('./site-management.service');
var router_1 = require('@angular/router');
var AddSiteComponent = (function () {
    function AddSiteComponent(siteService, router) {
        this.siteService = siteService;
        this.router = router;
    }
    AddSiteComponent.prototype.add = function () {
        this.siteService.addSite({ id: 0, name: this.siteName });
        this.router.navigate(['/list']);
    };
    AddSiteComponent = __decorate([
        core_1.Component({
            selector: 'add-site-view',
            templateUrl: 'app/add-site.template.html',
            styles: ["\n\th3{\n\t\tfont-weight: bold;\n\t\tcolor: maroon;\n\t}\n\t"]
        }), 
        __metadata('design:paramtypes', [site_management_service_1.SiteManagementService, router_1.Router])
    ], AddSiteComponent);
    return AddSiteComponent;
}());
exports.AddSiteComponent = AddSiteComponent;
//# sourceMappingURL=add-site.component.js.map