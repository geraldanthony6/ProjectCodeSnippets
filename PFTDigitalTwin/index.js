require('dotenv').config({path: './.env'})
console.log("MONGO_URI:", process.env.MONGO_URI)

const express = require('express')
const mongoose = require('mongoose')
const bodyParser = require('body-parser')

const app = express()

app.use(bodyParser.json())
app.use(bodyParser.urlencoded({extended: true}))

const MONGO_URI = process.env.MONGO_URI

mongoose.connect(MONGO_URI, {authSource: "admin"}).then(() => console.log("MongoDB Connected"))
.catch(err => console.error("MongoDB Coection Error:", err))


const UserSchema = new mongoose.Schema({
    age: Number,
    name: String
})

var RoomSchema = new mongoose.Schema({
    RoomFloor: {type: String},
    RoomGUID: {type: String,},
    RoomName: {type: String},
    RoomNumber: {type: String},
    RoomSqFootage: {type: Number},
    Department: {type: String},
    RoomOwners: [String],
    RoomType: {type: String},
});

const UserModel = mongoose.model("users", UserSchema)
var RoomModel = mongoose.model("rooms", RoomSchema)

app.get("/getUsers", async (req, res) => {
    try {
        const users = await UserModel.find()
        res.json(users)
    } catch(error) {
        res.status(500).json({error: "Failed to fetch users"})
    }
})


app.get("/getRoom/:RoomGUID", async (req, res) => {
    try {
        const rooms = await RoomModel.findOne({RoomGUID: req.params.RoomGUID}, {RoomGUID: 0, _id: 0})
        res.json(rooms)
    } catch(error) {
        res.status(500).json({error: "Failed to fetch rooms"})
    }
})

app.get("/getRoomsHUD/:RoomGUID", async (req, res) => {
    try {
        const rooms = await RoomModel.findOne({RoomGUID: req.params.RoomGUID}, {RoomFloor: 1, RoomNumber: 1, RoomName: 1, _id: 0})
        res.json(rooms)
    } catch(error) {
        res.status(500).json({error: "Failed to fetch rooms"})
    }
})


RoomModel.find().select('room_number').then(rooms => {
    console.log(rooms);
}).catch(err => {
    console.log(err);
});


app.post("/post", async(req, res) => {
    console.log("inside post function")
    
    const newRoom = new RoomModel({
        RoomFloor:req.body.RoomFloor,
        RoomGUID:req.body.RoomGUID,
        RoomName:req.body.RoomName,
        RoomNumber:req.body.RoomNumber,
        RoomSqFootage:req.body.RoomSqFootage,
        Department:req.body.Department,
        RoomOwners:req.body.RoomOwners,
        RoomType:req.body.RoomType,
    });

    const val = await newRoom.save();
    res.json(val);
});

app.listen(3000, () => {
    console.log("Server is Running")
})