FROM pqclean/ci-container:amd64

RUN apt-get update && apt-get install -y python3-jinja2 wget unzip unifdef

RUN adduser pqcuser
USER pqcuser

WORKDIR /home/pqcuser
RUN git clone -b hqc-20240223 --depth=1 https://github.com/SWilson4/package-pqclean.git package-pqclean
WORKDIR ./package-pqclean/hqc
RUN ./checkout.sh
RUN ./package.sh

CMD bash
