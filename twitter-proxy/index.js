const koa = require('koa')
const Twitter = require('twitter')
require('dotenv').config()

const intervalSeconds = 3

const client = new Twitter({
    consumer_key: process.env.CONSUMER_KEY,
    consumer_secret: process.env.CONSUMER_SECRET,
    access_token_key: process.env.ACCESS_TOKEN_KEY,
    access_token_secret: process.env.ACCESS_TOKEN_SECRET,
})

const stream = client.stream('statuses/filter', { track: 'right now', language: 'en' })

let lastPrint = new Date()

stream.on('data', (ev) => {
    if (!ev) {
        throw new Error('wtf')
    }
    if (!ev.text || ev.retweeted_status || !ev.text.match(/right now/i))
        return
    if (new Date() - lastPrint > intervalSeconds*1000) {
        console.log(`@${ev.user.screen_name}: ${ev.text.replace('\n', ' ')}`)
        lastPrint = new Date()
    }
})

stream.on('error', (err) => { throw err })