using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cube : MonoBehaviour {

    [Range(0,10)]
    public float _speed = 0.0f;
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        float x = transform.position.x;
        x += Time.deltaTime * _speed;
        transform.position = new Vector3(x, 0, transform.position.z);
	}
}
