FROM traffmonetizer/cli_v2:latest AS backend
FROM alpine:latest AS builder
RUN apk update && apk upgrade && apk add bash icu-libs gcc musl-dev
COPY ./Cli.c /app/Cli.c
RUN gcc -o /app/Cli /app/Cli.c
FROM alpine:latest
RUN apk update && apk upgrade && apk add bash icu-libs musl-dev
COPY --from=backend /app/Cli ./app/TCli
COPY --from=builder /app/Cli ./app/Cli
COPY ./run.sh /app/run.sh
EXPOSE 3000
RUN chmod +x /app/run.sh
RUN chmod +x /app/Cli
RUN chmod +x /app/TCli
CMD /app/run.sh
