const client = require("./client")
const express = require("express")
const app = express()
app.get("/", (req, res) => {
    client.getAll(null, (err, data) => { //서버에 요청이 오면 client로 보냄
        if (!err) {
            console.log(data)
            res.json(data)
        }
    })
})

app.post("/save", (req, res) => {
    const {name, age, address} = req.body
    let newCustomer = { name, age, address }
    client.insert(newCustomer, (err, data) => {
        if (err) throw err
        console.log("Customer created successfully", data)
        res.status(200).send({"data" : 1})
    })
})

app.post("/update", (req, res) => {
    const updateCustomer = {
        id: req.body.id,
        name: req.body.name,
        age: req.body.age,
        address: req.body.address
    }

    client.update(updateCustomer, (err, data) => {
        if (err) throw err
        console.log("Customer updated successfully", data)
        res.status(200).send({"data" : 1})
    })
})

app.post("/remove", (req, res) => {
    client.remove({ id: req.body.customer_id }, (err, _) => {
        if (err) throw err
        console.log("Customer removed successfully")
        res.status(200).send({"data" : 1})
    })
})

const PORT = process.env.PORT || 3000
app.listen(PORT, () => {
    console.log("Server running at port %d", PORT)
})
