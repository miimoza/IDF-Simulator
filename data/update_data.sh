echo "Updating data..."

mkdir stations
mkdir stations/metros
mkdir stations/rers
mkdir stations/tramways
mkdir stations/bus

api="https://api-ratp.pierre-grimaud.fr/v3"
wget -O "lines.json" "${api}/lines"

for t in metros rers tramways bus; do
    for i in $(seq 0 $(( "$(jq -r ".result.$t | length" lines.json)" - 1))); do
        
        code="$(jq -r ".result.$t[$i].code" lines.json )"
        echo "t:$t i:$i code:$code"
        echo "--> wget ${api}/stations/$t/$code" 
        wget -O "stations/$t/${code}.json" "${api}/stations/$t/$code" 
    done
done
