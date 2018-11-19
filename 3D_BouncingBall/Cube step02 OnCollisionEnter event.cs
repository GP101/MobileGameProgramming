using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cube : MonoBehaviour {

    //[Range(-10,10)]
    public float _speed = 0.0f;
	// Use this for initialization
	void Start () {
        //StartCoroutine(WaitAndGo(1.0f));
	}
	
	// Update is called once per frame
	void Update () {
        float x = transform.position.x;
        x += Time.deltaTime * _speed;
        transform.position = new Vector3(x, transform.position.y, transform.position.z);
	}

    void OnCollisionEnter(Collision collision)
    {
        _speed = -_speed;
    }
    private IEnumerator WaitAndGo(float waitTime)
    {
        while (true)
        {
            float x = transform.position.x;
            x += Time.deltaTime * _speed;
            transform.position = new Vector3(x, 0, transform.position.z);
            yield return new WaitForSeconds(waitTime);
        }
    }
}
