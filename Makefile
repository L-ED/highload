.PHONY: rest-image
rest-image:
	docker build --rm --no-cache -t rest-service:hl -f docker/rest-service/Dockerfile .
