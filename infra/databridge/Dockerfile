FROM node:16
WORKDIR /usr/src/app
copy package*.json ./
RUN npm install
COPY server.js ./
ADD static ./
