# Shortify AI

A scalable and intelligent URL shortening service built using C++, Crow, and Machine Learning concepts.

## Features

* URL shortening API
* RESTful backend services
* JSON request/response handling
* Multithreaded HTTP server
* Scalable backend architecture
* AI/ML integration planned
* Redis caching planned
* Analytics system planned

## Current APIs

### Health Check

```http
GET /health
```

Response:

```json
"Shortify AI Backend Running"
```

---

### Shorten URL

```http
POST /shorten
```

Request:

```json
{
    "url":"https://google.com"
}
```

Response:

```json
{
    "original_url":"https://google.com",
    "short_url":"abc123"
}
```

---

## Tech Stack

* C++
* Crow Framework
* Asio
* REST APIs
* JSON
* Git & GitHub

## Planned Features

* Base62 URL encoding
* Database integration
* Redis caching
* URL analytics
* QR code generation
* Malicious URL detection using ML
* Docker deployment
* Microservices architecture

## Project Structure

```txt
backend-cpp/
ml-service/
docs/
docker/
scripts/
```

## Status

MVP backend server successfully implemented and running locally.
