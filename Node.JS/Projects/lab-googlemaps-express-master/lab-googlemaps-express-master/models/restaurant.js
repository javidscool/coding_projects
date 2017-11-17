const mongoose = require('mongoose');
const Schema = mongoose.Schema;

//we need to do this because type is a reserved word
//so we need to let the code know that here it's a string
const RestaurantSchema = new Schema({
	name: String,
	description: String,
	location: { 
		type: { type: String }, 
		coordinates: [Number] 
	}
});

RestaurantSchema.index({ location: '2dsphere' });

const Restaurant = mongoose.model('Restaurant', RestaurantSchema);
module.exports = Restaurant;
