const PROTO_PATH = "./customers.proto"
const grpc = require("@grpc/grpc-js")
const protoLoader = require("@grpc/proto-loader")
const packageDefinition = protoLoader.loadSync(PROTO_PATH, {
    keepCase: true,
    longs: String,
    enums: String,
    arrays: true
})

const CustomerService = grpc.loadPackageDefinition(packageDefinition).CustomerService
const client = new CustomerService(
    "localhost:12010",
    grpc.credentials.createInsecure()
)

module.exports = client
