echo "Updating data..."

mkdir data/stations
mkdir data/stations/metros
mkdir data/stations/rers
mkdir data/stations/tramways
mkdir data/stations/bus

echo "Get stations postions"
wget -O "data/stations_position.csv" "https://data.iledefrance-mobilites.fr/explore/dataset/emplacement-des-gares-idf/download/?format=csv"


api="https://api-ratp.pierre-grimaud.fr/v3"
wget -O "data/lines.json" "${api}/lines"
cat "data/lines.json" | jq -r '.result[] | [.[].code] | @csv' > "data/lines.csv"

for t in metros rers tramways bus; do
    for i in $(seq 0 $(( "$(jq -r ".result.$t | length" data/lines.json)" - 1))); do

        code="$(jq -r ".result.$t[$i].code" data/lines.json )"
        echo "t:$t i:$i code:$code"
        echo "--> wget ${api}/stations/$t/$code"
        wget -O "data/stations/$t/${code}.json" "${api}/stations/$t/$code"
        echo "Converting Json to CSV"
        cat "data/stations/$t/${code}.json" | jq -r '.result[][] | [.name,.slug] | @csv' > "data/stations/$t/${code}.csv"
    done
done
