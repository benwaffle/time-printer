const Koa = require('koa')
const Router = require('koa-router')

const app = new Koa()
const router = new Router()

const Twitter = require('twitter')
require('dotenv').config()

const client = new Twitter({
    consumer_key: process.env.CONSUMER_KEY,
    consumer_secret: process.env.CONSUMER_SECRET,
    access_token_key: process.env.ACCESS_TOKEN_KEY,
    access_token_secret: process.env.ACCESS_TOKEN_SECRET,
})

const stream = client.stream('statuses/filter', { track: 'trump', language: 'en' })

let latestTweet = null;

function stripUnicode(str) {
    return Array.from(str).filter(x => x.charCodeAt() < 128).join('')
}

function stripUrls(str) {
    return str.replace(/https?:\/\/[A-Za-z0-9\.-\/]+/ig, '')
}

stream.on('data', (ev) => {
    if (!ev) {
        // throw new Error('wtf')
        return
    }
    if (!ev.text || ev.retweeted_status || ev.truncated) // || !ev.text.match(/right now/i) || ev.text.match(/[wc]ould/i))
        return

    latestTweet = stripUrls(stripUnicode(`@${ev.user.screen_name}: ${ev.text.replace(/\n/g, ' ')}`)).trim()
})

stream.on('error', (err) => { throw err })

router.get('/', (ctx) => {
    ctx.body = latestTweet
})

app
  .use(router.routes())
  .use(router.allowedMethods())
  .listen(3000)

console.log('http://localhost:3000')
