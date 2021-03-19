const Pool = require('pg').Pool
const pool = new Pool({
  user: 'iajtmsqjbnwmzn',
  host: 'ec2-54-145-249-177.compute-1.amazonaws.com',
  database: 'd69othoj62pqo3',
  password: '90473e5a3fcd258b258aa2643c4217020924cd592311ac7c4a91c6405e7ff7f6',
  port: 5432
})

const getUsers = (request, response) => {
  pool.query('SELECT * FROM users ORDER BY id ASC', (error, results) => {
    if (error) {
      throw error
    }
    response.status(200).json(results.rows)
  })
}

module.exports = {
  getUsers
}
